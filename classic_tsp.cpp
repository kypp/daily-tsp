#include <cstdio>

int n;
int l[25][25];
int d[1<<20][20];

inline int min(int a,int b){
    if(a>b)return b;
    return a;
}

int main(){
    for(int i=0;i<20;i++)for(int j=0;j<(1<<20);j++)d[j][i]=(1<<30);//nieskończoności
    scanf("%d",&n);
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)scanf("%d",&l[i][j]);

    unsigned int v,w,k;

    for(int i=0;i<n;i++){
        d[(1<<i)+1][i]=l[0][i];
    }

    for(int k=3;k<(1<<n);k+=2){
        for(v=1;v<n;v++)if(k!=(k|(1<<v))){//jeżeli v powiększy stan
            for(w=1;w<n;w++){
                if(w!=v&&d[k|(1<<v)][v]>d[k][w]+l[w][v])d[k|(1<<v)][v]=d[k][w]+l[w][v];
            }
        }
    }

    int wynik=(1<<30);
    for(int i=0;i<20;i++)if(wynik>d[(1<<n)-1][i]+l[i][0])wynik=d[(1<<n)-1][i]+l[i][0];

    printf("%d",wynik);

    /*for(k=0;k<(1<<n);k++){
        for(int i=0;i<n;i++)printf("%d ",d[k][i]==(1<<30)?-1:d[k][i]);
        printf("\n");
    }*/
    return 0;
}
