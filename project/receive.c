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

#define DESTMAC0	0xd0
#define DESTMAC1	0x67
#define DESTMAC2	0xe5
#define DESTMAC3	0x12
#define DESTMAC4	0x6f
#define DESTMAC5	0x8f

#define destination_ip 10.240.253.10


struct ifreq ifreq_c,ifreq_i,ifreq_ip; /// for each ioctl keep diffrent ifreq structure otherwise error may come in sending(sendto )

int sock_raw;
int total_len=0;

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
int main(){

    FILE *logfile=fopen("log.txt","w");
    //opening a raw socket
    int sock_r;
    sock_r=socket(AF_PACKET,SOCK_RAW,IPPROTO_UDP);
    if(sock_r<0)
    {
        printf("error in socket\n");
        return -1;
    }

    //receive packet
    unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
    memset(buffer,0,65536);
    struct sockaddr saddr;
    int saddr_len = sizeof (saddr);
    int buflen;
    //Receive a network packet and copy in to buffer
    buflen=recvfrom(sock_r,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
    if(buflen<0)
    {
        printf("error in reading recvfrom function\n");
        return -1;
    }

    //extract eternet header
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    fprintf(logfile,"\nEthernet Header\n");
    fprintf(logfile,"\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
    fprintf(logfile,"\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
    fprintf(logfile,"\t|-Protocol : %d\n",eth->h_proto);

    //extract ip header

    struct sockaddr_in source,dest;

    unsigned short iphdrlen;
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;
    
    fprintf(logfile, "\t|-Version : %d\n",(unsigned int)ip->version);
    
    fprintf(logfile , "\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
    
    fprintf(logfile , "\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
    
    fprintf(logfile , "\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
    
    fprintf(logfile , "\t|-Identification : %d\n",ntohs(ip->id));
    
    fprintf(logfile , "\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
    
    fprintf(logfile , "\t|-Protocol : %d\n",(unsigned int)ip->protocol);
    
    fprintf(logfile , "\t|-Header Checksum : %d\n",ntohs(ip->check));
    
    fprintf(logfile , "\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
    
    fprintf(logfile , "\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));

    //extract transport layer header

    /* getting actual size of IP header*/
    iphdrlen = ip->ihl*4;
    /* getting pointer to udp header*/
    struct udphdr *udp=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

    fprintf(logfile , "\t|-Source Port : %d\n" , ntohs(udp->source));
    fprintf(logfile , "\t|-Destination Port : %d\n" , ntohs(udp->dest));
    fprintf(logfile , "\t|-UDP Length : %d\n" , ntohs(udp->len));
    fprintf(logfile , "\t|-UDP Checksum : %d\n" , ntohs(udp->check));

    //extracting data

    unsigned char *data = (buffer + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

    int remaining_data = buflen - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
 
    for(int i=0;i<remaining_data;i++)
    {
        if(i!=0 && i%16==0)
        fprintf(logfile,"\n");
        fprintf(logfile," %.2X ",data[i]);
    }

    /*************sending packet*******************/

    sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
    if(sock_raw == -1){
        printf("error in socket");
    }

    get_eth_index();
    get_mac();
    get_ip();

    unsigned char *sendbuff;

    sendbuff=(unsigned char*)malloc(64); // increase in case of more data
    memset(sendbuff,0,64);

    struct ethhdr *eth = (struct ethhdr *)(sendbuff);
 
    eth->h_source[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
    eth->h_source[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
    eth->h_source[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
    eth->h_source[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
    eth->h_source[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
    eth->h_source[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);
    
    /* filling destination mac. DESTMAC0 to DESTMAC5 are macro having octets of mac address. */
    eth->h_dest[0] = DESTMAC0;
    eth->h_dest[1] = DESTMAC1;
    eth->h_dest[2] = DESTMAC2;
    eth->h_dest[3] = DESTMAC3;
    eth->h_dest[4] = DESTMAC4;
    eth->h_dest[5] = DESTMAC5;

    eth->h_proto = htons(ETH_P_IP); //means next header will be IP header
 
    /* end of ethernet header */
    total_len+=sizeof(struct ethhdr);

    struct iphdr *iph = (struct iphdr*)(sendbuff + sizeof(struct ethhdr));
    
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 16;
    iph->id = htons(10201);
    iph->ttl = 64;
    iph->protocol = 17;
    iph->saddr = inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr)));
    iph->daddr = inet_addr("destination_ip"); // put destination IP address
    
    total_len += sizeof(struct iphdr);

    struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct iphdr) + sizeof(struct ethhdr));
 
    uh->source = htons(23451);
    uh->dest = htons(23452);
    uh->check = 0;
    
    total_len+= sizeof(struct udphdr);

    sendbuff[total_len++] = 0xAA;
    sendbuff[total_len++] = 0xBB;
    sendbuff[total_len++] = 0xCC;
    sendbuff[total_len++] = 0xDD;
    sendbuff[total_len++] = 0xEE;

    uh->len = htons((total_len - sizeof(struct iphdr) - sizeof(struct ethhdr)));
    //UDP length field
    iph->tot_len = htons(total_len - sizeof(struct ethhdr));
    //IP length field

    
    iph->check = checksum((unsigned short*)(sendbuff + sizeof(struct ethhdr)), (sizeof(struct iphdr)/2));

    struct sockaddr_ll sadr_ll;
    sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
    sadr_ll.sll_halen = ETH_ALEN; // length of destination mac address
    sadr_ll.sll_addr[0] = DESTMAC0;
    sadr_ll.sll_addr[1] = DESTMAC1;
    sadr_ll.sll_addr[2] = DESTMAC2;
    sadr_ll.sll_addr[3] = DESTMAC3;
    sadr_ll.sll_addr[4] = DESTMAC4;
    sadr_ll.sll_addr[5] = DESTMAC5;


    int send_len = sendto(sock_raw,sendbuff,64,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
    if(send_len<0)
    {
        printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
        return -1;
    
    }
}