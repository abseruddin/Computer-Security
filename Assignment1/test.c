#include <stdint.h>
#include<stdio.h>
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

void initialize_aes_sbox(uint8_t sbox[]) {
	uint8_t p = 1, q = 1;
	
	/* loop invariant: p * q == 1 in the Galois field */
    int num=0;
	do {
		/* multiply p by 3 */
        // printf("%d\n",p);
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x11B : 0);
        // printf("%d\n",p);
		/* divide q by 3 (equals multiplication by 0xf6) */
        // printf("%d\n",q);
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;
        // printf("%d\n",q);
		/* compute the affine transformation */
        // printf("%d\n",ROTL8(q, 1));
        // printf("%d\n",ROTL8(q, 2));
        // printf("%d\n",ROTL8(q, 3));
        // printf("%d\n",ROTL8(q, 4));
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
        // printf("%d\n",xformed);
		sbox[p] = xformed ^ 0x63;
        printf("%d %d\n",p,sbox[p]);
        num++;
	} while (p != 1&&num>=0);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
}
main(){
    uint8_t sbox[256];
    initialize_aes_sbox(sbox);
    // for(int i=0;i<16;i++){
    //     for(int j=0;j<16;j++){
    //         printf("<<sbox[i*16+j]<<" ";
    //     }
    //     printf("<<\n");
    // }
}