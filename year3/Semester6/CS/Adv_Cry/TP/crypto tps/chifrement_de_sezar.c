#include<stdio.h>
#include<string.h>

    int shift;


int main(){
    char message [10000];
    char T1 [26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
    char T2 [26];
    printf("enter the key \n");
    scanf("%d",&shift);
    for(int i=shift;i<=26-shift;i++){
            int k=0;
            T2[k]=T1[i];
            k++;


        for(int j=0;j<shift;j++){

        T2[i]=T1[j];

        }
        T2[i]=T1[i];

        /*
        if(shift+i>26){
                for(int j=26;j>26-i;j--){
                    T2[i]=T1[26-j];
                    if(shift == 26-j){
                        break;
                    }
                    }
        }*/
    }
    printf("enter your message :");
    scanf("%s",&message);
    printf("%s",T2);




}
