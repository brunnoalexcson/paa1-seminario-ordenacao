#include <bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;

//We seek the perfect algorithm, but life's best answers just don't compile.
//By Xaulin_Du_Grau. ;-;

signed main(){
    int n;
    fds:
    cout<<"Digite o tamanho do vetor: ";
    cin>>n;
    if(n<=0 or n>1e7){
        cout<<"Va te lascar digita um numero de gente ;-;"<<endl;
        this_thread::sleep_for(chrono::seconds(4));
        system("cls");
        goto fds;
    }
    vector<int> ve(n);
    cout<<"Acora digite os elementos do vetor: ";
    for(int &i: ve) cin>>i;
    auto merge = [&] (this auto &&merge, int l, int r, auto &a)->void{
        if(l>=r) return;
        int m=(l+r)>>1;
        merge(l, m, a);
        merge(m+1, r, a);
        int tam=r-l+1;
        vector<int> aux(tam);
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
        return;
    };
    merge(0, n-1, ve);
    cout<<"Seu vetor ordenado eh:"<<endl;
    for(int &i: ve) cout<<i<<' ';
    cout<<endl;
    exit (0);
}
