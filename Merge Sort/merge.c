#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <windows.h>

//We seek the perfect algorithm, but life's best answers just don't compile.
//By Xaulin_Du_Grau. ;-;

void merge(int l, int r, int *a){
    if(l>=r) return;
    int m=(l+r)>>1;
    merge(l, m, a);
    merge(m+1, r, a);
    int tam=r-l+1;
    int *aux=malloc(tam*sizeof(int));
    int i=l;
    int j=m+1;
    int to=0;
    while(i<=m and j<=r){
        if(a[i]<=a[j]){
            aux[to++]=a[i++];
        }
        else aux[to++]=a[j++];
    }
    while(i<=m) aux[to++]=a[i++];
    while(j<=r) aux[to++]=a[j++];
    for(int i=0 ; i<tam ; i++){
        a[i+l]=aux[i];
    }
    free(aux);
    return;
}

signed main(){
    int n;
    fds:
    printf("Digite o tamanho do vetor: ");
    scanf("%d", &n);
    if(n<=0 or n>1e7){
        printf("Va te lascar digita um numero de gente ;-;\n");
        Sleep(4000);
        system("cls");
        goto fds;
    }
    int *ve=malloc(n*sizeof(int));
    printf("Acora digite os elementos do vetor: ");
    for(int i=0 ; i<n ; i++) scanf("%d", &ve[i]);
    merge(0, n-1, ve);
    printf("Seu vetor ordenado eh:\n");
    for(int i=0 ; i<n ; i++) printf("%d ", ve[i]);
    printf("\n");
    free(ve);
    exit (0);
}
