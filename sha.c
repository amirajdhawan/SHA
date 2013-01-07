#include<stdio.h>
#include<string.h>
#include"sha_func.h"

void main(char **args,int argv){
    unsigned char msg[64];
    int i;
    unsigned abcdef[5];
    unsigned char msg_len[4];
    SHAContext *con;
    con=(SHAContext *)malloc(sizeof(SHAContext));
    //msg=(unsigned char *) malloc(64*sizeof(unsigned char));
    init(con);
   /* printf("Enter Message block: ");
    scanf("%64s",msg);
   */
   //msg="qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopas";
    FILE *fp;
    fp = fopen("data.txt","r");
    while(fgets(msg,64,fp) != NULL);
    printf("\n Msg: %s",msg);
    //unsigned char *actmsg;
    //actmsg = (unsigned char *) malloc((strlen(msg))*sizeof(unsigned char));
    //printf("\n\nLength of Message: %d",strlen(msg));
    //char *temp;
    //temp=(char *) malloc(sizeof(char));
    //temp[0]=0x80;
    //strcpy(actmsg,msg);
    int len=strlen(msg);
    //actmsg[len-1] = 0;
    //actmsg[len-1] = setbit(actmsg[len-1],7);
    //strcat(actmsg,temp);
    //actmsg[0]=0x79;
    //actmsg[len-1] = 0x79;
    msg[len-1]=0x80;
    //printf("\n\nspl1: %d",msg[len-1]);
    //actmsg[len-1] ++;
    //printf("\n%s",actmsg);
    //strcpy(msg,actmsg);
    for( i = strlen(msg); i < 64; i++){
        msg[i] = 0;
    }
    int l = (len-1)*8;
    //printf("\n\nMsg_len: ");
    convert_int_char(l,msg_len);
    for(i = 3; i >= 0; i--){
        msg[63-i] = msg_len[i];
    }
    printf("\n\n");
    for( i = 0; i < 64; i++)
        printf("%X ",msg[i]);
    unsigned int words[80];
    /*for( i = 0; i < 80; i++)
        words[i]=0;
    */
    init_words(words);
    /*for( i = 0; i < 64; i++){
        words[i/4]+=msg[i];
    }*/
    init_words_msg(words,msg);
    init_words_remain(words);
    printf("\nWords: ");
    for(i = 0; i < 80; i++)
        printf("%X,  ",words[i]);
//    init_words_remain(words);
    /*int cls=4;
    cls=circular_left_shift(cls,1);
    printf("\n\nCLR value-> %d",cls);*/
    init_intermediate_hash(abcdef, con);
    /*for( i = 0; i < 5; i++)
        printf("\n\n%X",abcdef[i]);*/
    calc_SHA(abcdef, words, con);
    display_hash(con);
}
