#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<errno.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>

#include<net/if.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/if_ether.h>
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>

#include<linux/if_packet.h>

#include<arpa/inet.h>

#define SRCMAC0	0x1e
#define SRCMAC1	0x2e
#define SRCMAC2	0xfa
#define SRCMAC3	0x79
#define SRCMAC4	0xa1
#define SRCMAC5	0xf4

#define destination_ip 192.168.0.10

struct sockaddr_in source,dest;
struct ifreq ifreq_c,ifreq_i,ifreq_ip; /// for each ioctl keep diffrent ifreq structure otherwise error may come in sending(sendto )

int sock_raw;
int total_len=0;
FILE *logfile;

void get_eth_index()
{
	memset(&ifreq_i,0,sizeof(ifreq_i));
	strncpy(ifreq_i.ifr_name,"h1-eth0",IFNAMSIZ-1);

	if((ioctl(sock_raw,SIOCGIFINDEX,&ifreq_i))<0)
		printf("error in index ioctl reading");

	printf("index=%d\n",ifreq_i.ifr_ifindex);

}

void get_mac(){
    struct ifreq ifreq_c;
    memset(&ifreq_c,0,sizeof(ifreq_c));
    strncpy(ifreq_c.ifr_name,"h1-eth0",IFNAMSIZ-1);//giving name of Interface
    
    if((ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
    	printf("error in SIOCGIFHWADDR ioctl reading");
}

void get_ip(){
    struct ifreq ifreq_ip;
    memset(&ifreq_ip,0,sizeof(ifreq_ip));
    strncpy(ifreq_ip.ifr_name,"h1-eth0",IFNAMSIZ-1);//giving name of Interface
    if(ioctl(sock_raw,SIOCGIFADDR,&ifreq_ip)<0) //getting IP Address
    {
        printf("error in SIOCGIFADDR \n");
    }
}

unsigned short checksum(unsigned short* buff, int _16bitword)
{
    unsigned long sum;
    for(sum=0;_16bitword>0;_16bitword--)
    sum+=htons(*(buff)++);
    sum = ((sum >> 16) + (sum & 0xFFFF));
    sum += (sum>>16);
    return (unsigned short)(~sum);
}
FILE *logfile;
int main(){

    int saddr_size , data_size;
    struct sockaddr saddr;
	struct in_addr in;
	
	unsigned char *buffer = (unsigned char *)malloc(65536); //Its Big!
	
	logfile=fopen("log.txt","w");
	if(logfile==NULL) printf("Unable to create file.");
	printf("Starting...\n");
	//Create a raw socket that shall sniff
	sock_raw = socket(AF_PACKET , SOCK_RAW , htons(ETH_P_ALL));
	if(sock_raw < 0)
	{
		printf("Socket Error\n");
		return 1;
	}
    
	saddr_size = sizeof saddr;
	//Receive a packet
	data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , &saddr_size);
	if(data_size <0 )
	{
		printf("Recvfrom error , failed to get packets\n");
		return 1;
	}
	//Now process the packet

	unsigned short iphdrlen;

	
	fprintf(logfile,"\n\n***********************UDP Packet*************************\n");
	
	
	fprintf(logfile,"\n");
	//extract eternet header
	fprintf(logfile,"Ethernet Header\n");
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    fprintf(logfile,"\nEthernet Header\n");
    fprintf(logfile,"\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
    fprintf(logfile,"\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
    fprintf(logfile,"\t|-Protocol : %d\n",eth->h_proto);

    //extract ip header

    /*struct sockaddr_in source,dest;

    unsigned short iphdrlen;
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;*/
    
    // print ip header
	struct iphdr *iph = (struct iphdr *)(buffer+ sizeof(struct ethhdr));
	iphdrlen = iph->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;
	fprintf(logfile,"IP Header\n");
	fprintf(logfile,"   |-IP Version        : %d\n",(unsigned int)iph->version);
	fprintf(logfile,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
	fprintf(logfile,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
	fprintf(logfile,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
	fprintf(logfile,"   |-Identification    : %d\n",ntohs(iph->id));
	//fprintf(logfile,"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
	//fprintf(logfile,"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
	//fprintf(logfile,"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
	fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
	fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)iph->protocol);
	fprintf(logfile,"   |-Checksum : %d\n",ntohs(iph->check));
	fprintf(logfile,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
	fprintf(logfile,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
	
	// print udp header
	struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen+sizeof(struct ethhdr));			
	
	fprintf(logfile,"\nUDP Header\n");
	fprintf(logfile,"   |-Source Port      : %d\n" , ntohs(udph->source));
	fprintf(logfile,"   |-Destination Port : %d\n" , ntohs(udph->dest));
	fprintf(logfile,"   |-UDP Length       : %d\n" , ntohs(udph->len));
	fprintf(logfile,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
	
	//print remaining data
	fprintf(logfile,"\nData\n");
    unsigned char *data = (buffer + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

    int remaining_data = data_size - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
 
    for(int i=0;i<remaining_data;i++)
    {
        if(i!=0 && i%16==0)
        	fprintf(logfile,"\n");
        fprintf(logfile," %.2X ",data[i]);
    }
	
	fprintf(logfile,"\n");
	
    
    /*************sending packet*******************/

	get_eth_index();
    get_mac();
    get_ip();

    /*sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
    if(sock_raw == -1){
        printf("error in socket");
    }*/


    unsigned char *sendbuff;

    sendbuff=(unsigned char*)malloc(65536); // increase in case of more data
    memset(sendbuff,0,65536);

    struct ethhdr *eth1 = (struct ethhdr *)(sendbuff);
 
    eth1->h_source[0] = SRCMAC0;//(unsigned char)(0xf2);//(ifreq_c.ifr_hwaddr.sa_data[0]);
    eth1->h_source[1] = SRCMAC1;//(unsigned char)(0x79);//(ifreq_c.ifr_hwaddr.sa_data[1]);
    eth1->h_source[2] = SRCMAC2;//(unsigned char)(0x93);//(ifreq_c.ifr_hwaddr.sa_data[2]);
    eth1->h_source[3] = SRCMAC3;//(unsigned char)(0x9c);//(ifreq_c.ifr_hwaddr.sa_data[3]);
    eth1->h_source[4] = SRCMAC4;//(unsigned char)(0x08);//(ifreq_c.ifr_hwaddr.sa_data[4]);
    eth1->h_source[5] = SRCMAC5;//(unsigned char)(0x6a);//(ifreq_c.ifr_hwaddr.sa_data[5]);
    
    /* filling destination mac. DESTMAC0 to DESTMAC5 are macro having octets of mac address. */
    eth1->h_dest[0] = eth->h_source[0];//DESTMAC0;
    eth1->h_dest[1] = eth->h_source[1];//DESTMAC0;
    eth1->h_dest[2] = eth->h_source[2];//DESTMAC0;
    eth1->h_dest[3] = eth->h_source[3];//DESTMAC0;
    eth1->h_dest[4] = eth->h_source[4];//DESTMAC0;
    eth1->h_dest[5] = eth->h_source[5];//DESTMAC0;

    eth1->h_proto = htons(ETH_P_IP); //means next header will be IP header
 
    /* end of ethernet header */
    total_len+=sizeof(struct ethhdr);

    iph = (struct iphdr*)(sendbuff + sizeof(struct ethhdr));
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 16;
    iph->id = htons(10201);
    iph->ttl = 128;
    iph->protocol = 17;
    iph->saddr = inet_addr("10.0.0.1");//(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr)));
    iph->daddr = inet_addr("255.255.255.255"); // put destination IP address
    
    total_len += sizeof(struct iphdr);

    struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct iphdr) + sizeof(struct ethhdr));
 
    uh->source = htons(67);
    uh->dest = htons(68);
    uh->check = 0;
    
    total_len+= sizeof(struct udphdr);

	int l1=total_len;
	/*printf("%d\n",remaining_data);
	for(int i=0;i<remaining_data;i++)
    {
        sendbuff[total_len++]=data[i];
    }*/
    sendbuff[l1++] = 0x02;//operation
    sendbuff[l1++] = 0x01;//hardwarw type
    sendbuff[l1++] = 0x06;//hardware length
    sendbuff[l1++] = 0x00;//hops
    
    sendbuff[l1++] = data[4];//transaction id
    sendbuff[l1++] = data[5];
    sendbuff[l1++] = data[6];
    sendbuff[l1++] = data[7];
    
    sendbuff[l1++] = 0x00;//seconds
    sendbuff[l1++] = 0x00;
    
    sendbuff[l1++] = 0x00;//flags
    sendbuff[l1++] = 0x00;
    
    sendbuff[l1++] = 0x00;//client ip address
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    
    sendbuff[l1++] = 0x0A;//your client(ip) address
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x0A;
    
    //next server and relay agent ip address
    sendbuff[l1++] = 0x0A;//next server(ip) address
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x01;
    for(int i=0;i<4;i++){
    	sendbuff[l1++]=0x00;
    }
    
    sendbuff[l1++] = eth->h_source[0];//client mac address
    sendbuff[l1++] = eth->h_source[1];
    sendbuff[l1++] = eth->h_source[2];
    sendbuff[l1++] = eth->h_source[3];
    sendbuff[l1++] = eth->h_source[4];
    sendbuff[l1++] = eth->h_source[5];
    
    for(int i=0;i<202;i++){
    	sendbuff[l1++]=0x00;
    }
    
    sendbuff[l1++] = 0x63;//magic cookie:dhcp
    sendbuff[l1++] = 0x82;
    sendbuff[l1++] = 0x53;
    sendbuff[l1++] = 0x63;
    
    sendbuff[l1++] = 0x35;//options(53):dhcp message type
    sendbuff[l1++] = 0x01;
    sendbuff[l1++] = 0x02;
    
    sendbuff[l1++] = 0x01;//options(1):subnet mask
    sendbuff[l1++] = 0x04;
    sendbuff[l1++] = 0xff;
    sendbuff[l1++] = 0xff;
    sendbuff[l1++] = 0xff;
    sendbuff[l1++] = 0x00;
    
    sendbuff[l1++] = 0x3a;//options(58):renewal time value
    sendbuff[l1++] = 0x04;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x07;
    sendbuff[l1++] = 0x08;
    
    sendbuff[l1++] = 0x3b;//options(59):rebinding time value
    sendbuff[l1++] = 0x04;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x0c;
    sendbuff[l1++] = 0x4e;
    
    sendbuff[l1++] = 0x33;//options(51):ip address lease time
    sendbuff[l1++] = 0x04;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x0e;
    sendbuff[l1++] = 0x10;
    
    sendbuff[l1++] = 0x36;//options(54):dhcp server identifier
    sendbuff[l1++] = 0x04;
    sendbuff[l1++] = 0xc0;
    sendbuff[l1++] = 0xa8;
    sendbuff[l1++] = 0x00;
    sendbuff[l1++] = 0x01;
    
    sendbuff[l1++] = 0xff;//option end
    for(int i=0;i<32;i++){
    	sendbuff[l1++]=0x00;
    }
    total_len=l1;
    /*sendbuff[total_len++] = 0x0A;//server ip address
    sendbuff[total_len++] = 0x00;
    sendbuff[total_len++] = 0x00;
    sendbuff[total_len++] = 0x0A;
    
    sendbuff[total_len++] = 0x00;//gateway ip address
    sendbuff[total_len++] = 0x00;
    sendbuff[total_len++] = 0x00;
    sendbuff[total_len++] = 0x00;*/

    uh->len = htons((total_len - sizeof(struct iphdr) - sizeof(struct ethhdr)));
    //UDP length field
    iph->tot_len = htons(total_len - sizeof(struct ethhdr));
    //IP length field

    
    iph->check = htons(checksum((unsigned short*)(sendbuff + sizeof(struct ethhdr)), (sizeof(struct iphdr)/2)));

    struct sockaddr_ll sadr_ll;
    sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
    sadr_ll.sll_halen = ETH_ALEN; // length of destination mac address
    sadr_ll.sll_addr[0] = eth->h_source[0];//DESTMAC0;
    sadr_ll.sll_addr[1] = eth->h_source[1];//DESTMAC1;
    sadr_ll.sll_addr[2] = eth->h_source[2];//DESTMAC2;
    sadr_ll.sll_addr[3] = eth->h_source[3];//DESTMAC3;
    sadr_ll.sll_addr[4] = eth->h_source[4];//DESTMAC4;
    sadr_ll.sll_addr[5] = eth->h_source[5];//DESTMAC5;

	printf("%d",total_len);
    int send_len = sendto(sock_raw,sendbuff,total_len,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
    if(send_len<0)
    {
        printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
        return -1;
    
    }
    
    /********************************receiving request************************************************/
    total_len=0;
    l1=0;
	saddr_size = sizeof saddr;
	unsigned char *bufferRequest = (unsigned char *)malloc(65536);
	//Receive a packet
	data_size = recvfrom(sock_raw , bufferRequest , 65536 , 0 , &saddr , &saddr_size);
	if(data_size <0 )
	{
		printf("Recvfrom error , failed to get packets\n");
		return 1;
	}
	

	struct ethhdr *ethRequest = (struct ethhdr *)(bufferRequest);
	struct iphdr *iphRequest = (struct iphdr *)(bufferRequest+ sizeof(struct ethhdr));
	iphdrlen = iphRequest->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iphRequest->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iphRequest->daddr;

	struct udphdr *udphRequest = (struct udphdr*)(bufferRequest + iphdrlen+sizeof(struct ethhdr));
	unsigned char *dataRequest = (bufferRequest + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

	remaining_data = data_size - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

	/********************************sending ack************************************************/
	unsigned char *sendbuffRequest;

    sendbuffRequest=(unsigned char*)malloc(65536); // increase in case of more data
    memset(sendbuffRequest,0,65536);

    struct ethhdr *ethRequest1 = (struct ethhdr *)(sendbuffRequest);
 
    ethRequest1->h_source[0] = SRCMAC0;//(unsigned char)(0xf2);//(ifreq_c.ifr_hwaddr.sa_data[0]);
    ethRequest1->h_source[1] = SRCMAC1;//(unsigned char)(0x79);//(ifreq_c.ifr_hwaddr.sa_data[1]);
    ethRequest1->h_source[2] = SRCMAC2;//(unsigned char)(0x93);//(ifreq_c.ifr_hwaddr.sa_data[2]);
    ethRequest1->h_source[3] = SRCMAC3;//(unsigned char)(0x9c);//(ifreq_c.ifr_hwaddr.sa_data[3]);
    ethRequest1->h_source[4] = SRCMAC4;//(unsigned char)(0x08);//(ifreq_c.ifr_hwaddr.sa_data[4]);
    ethRequest1->h_source[5] = SRCMAC5;//(unsigned char)(0x6a);//(ifreq_c.ifr_hwaddr.sa_data[5]);
    
    /* filling destination mac. DESTMAC0 to DESTMAC5 are macro having octets of mac address. */
    ethRequest1->h_dest[0] = ethRequest->h_source[0];//DESTMAC0;
    ethRequest1->h_dest[1] = ethRequest->h_source[1];//DESTMAC0;
    ethRequest1->h_dest[2] = ethRequest->h_source[2];//DESTMAC0;
    ethRequest1->h_dest[3] = ethRequest->h_source[3];//DESTMAC0;
    ethRequest1->h_dest[4] = ethRequest->h_source[4];//DESTMAC0;
    ethRequest1->h_dest[5] = ethRequest->h_source[5];//DESTMAC0;

    ethRequest1->h_proto = htons(ETH_P_IP); //means next header will be IP header
 
    /* end of ethernet header */
    total_len+=sizeof(struct ethhdr);

    iphRequest = (struct iphdr*)(sendbuffRequest + sizeof(struct ethhdr));
    iphRequest->ihl = 5;
    iphRequest->version = 4;
    iphRequest->tos = 16;
    iphRequest->id = htons(10201);
    iphRequest->ttl = 128;
    iphRequest->protocol = 17;
    iphRequest->saddr = inet_addr("10.0.0.1");//(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr)));
    iphRequest->daddr = inet_addr("255.255.255.255"); // put destination IP address
    
    total_len += sizeof(struct iphdr);

    struct udphdr *uhRequest = (struct udphdr *)(sendbuffRequest + sizeof(struct iphdr) + sizeof(struct ethhdr));
 
    uhRequest->source = htons(67);
    uhRequest->dest = htons(68);
    uhRequest->check = 0;
    
    total_len+= sizeof(struct udphdr);

	l1=total_len;

    sendbuffRequest[l1++] = 0x02;//operation
    sendbuffRequest[l1++] = 0x01;//hardwarw type
    sendbuffRequest[l1++] = 0x06;//hardware length
    sendbuffRequest[l1++] = 0x00;//hops
    
    sendbuffRequest[l1++] = dataRequest[4];//transaction id
    sendbuffRequest[l1++] = dataRequest[5];
    sendbuffRequest[l1++] = dataRequest[6];
    sendbuffRequest[l1++] = dataRequest[7];
    
    sendbuffRequest[l1++] = 0x00;//seconds
    sendbuffRequest[l1++] = 0x00;
    
    sendbuffRequest[l1++] = 0x00;//flags
    sendbuffRequest[l1++] = 0x00;
    
    sendbuffRequest[l1++] = 0x00;//client ip address
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    
    sendbuffRequest[l1++] = 0x0A;//your client(ip) address
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x0A;
    
    //next server and relay agent ip address
    sendbuffRequest[l1++] = 0x0A;//next server(ip) address
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x01;
    for(int i=0;i<4;i++){
    	sendbuffRequest[l1++]=0x00;
    }
    
    sendbuffRequest[l1++] = ethRequest->h_source[0];//client mac address
    sendbuffRequest[l1++] = ethRequest->h_source[1];
    sendbuffRequest[l1++] = ethRequest->h_source[2];
    sendbuffRequest[l1++] = ethRequest->h_source[3];
    sendbuffRequest[l1++] = ethRequest->h_source[4];
    sendbuffRequest[l1++] = ethRequest->h_source[5];
    
    for(int i=0;i<202;i++){
    	sendbuffRequest[l1++]=0x00;
    }
    
    sendbuffRequest[l1++] = 0x63;//magic cookie:dhcp
    sendbuffRequest[l1++] = 0x82;
    sendbuffRequest[l1++] = 0x53;
    sendbuffRequest[l1++] = 0x63;
    
    sendbuffRequest[l1++] = 0x35;//options(53):dhcp message type
    sendbuffRequest[l1++] = 0x01;
    sendbuffRequest[l1++] = 0x05;
    
    sendbuffRequest[l1++] = 0x01;//options(1):subnet mask
    sendbuffRequest[l1++] = 0x04;
    sendbuffRequest[l1++] = 0xff;
    sendbuffRequest[l1++] = 0xff;
    sendbuffRequest[l1++] = 0xff;
    sendbuffRequest[l1++] = 0x00;
    
    sendbuffRequest[l1++] = 0x3a;//options(58):renewal time value
    sendbuffRequest[l1++] = 0x04;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x07;
    sendbuffRequest[l1++] = 0x08;
    
    sendbuffRequest[l1++] = 0x3b;//options(59):rebinding time value
    sendbuffRequest[l1++] = 0x04;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x0c;
    sendbuffRequest[l1++] = 0x4e;
    
    sendbuffRequest[l1++] = 0x33;//options(51):ip address lease time
    sendbuffRequest[l1++] = 0x04;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x0e;
    sendbuffRequest[l1++] = 0x10;
    
    sendbuffRequest[l1++] = 0x36;//options(54):dhcp server identifier
    sendbuffRequest[l1++] = 0x04;
    sendbuffRequest[l1++] = 0x0a;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x00;
    sendbuffRequest[l1++] = 0x01;
    
    sendbuffRequest[l1++] = 0xff;//option end
    for(int i=0;i<32;i++){
    	sendbuffRequest[l1++]=0x00;
    }
    total_len=l1;
    
    uhRequest->len = htons((total_len - sizeof(struct iphdr) - sizeof(struct ethhdr)));
    //UDP length field
    iphRequest->tot_len = htons(total_len - sizeof(struct ethhdr));
    //IP length field

    
    iphRequest->check = htons(checksum((unsigned short*)(sendbuffRequest + sizeof(struct ethhdr)), (sizeof(struct iphdr)/2)));

    struct sockaddr_ll sadr_llRequest;
    sadr_llRequest.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
    sadr_llRequest.sll_halen = ETH_ALEN; // length of destination mac address
    sadr_llRequest.sll_addr[0] = ethRequest->h_source[0];//DESTMAC0;
    sadr_llRequest.sll_addr[1] = ethRequest->h_source[1];//DESTMAC1;
    sadr_llRequest.sll_addr[2] = ethRequest->h_source[2];//DESTMAC2;
    sadr_llRequest.sll_addr[3] = ethRequest->h_source[3];//DESTMAC3;
    sadr_llRequest.sll_addr[4] = ethRequest->h_source[4];//DESTMAC4;
    sadr_llRequest.sll_addr[5] = ethRequest->h_source[5];//DESTMAC5;

	printf("%d",total_len);
    send_len = sendto(sock_raw,sendbuffRequest,total_len,0,(const struct sockaddr*)&sadr_llRequest,sizeof(struct sockaddr_ll));
    if(send_len<0)
    {
        printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
        return -1;
    
    }
}
