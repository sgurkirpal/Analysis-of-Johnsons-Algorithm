#include <bits/stdc++.h>
using namespace std;
#define MAX 999999
#define pii pair<int,int>

typedef struct node{
    int size;
    int cap;
    pair<int,int> *arr;
}myHeap;


int bin_parentIndex(int index)
{
    return (int)((index-1)/2);
}

int bin_leftChild(int index)
{
    return 2*index + 1;
}

int bin_rightChild(int index)
{
    return 2*index + 2;
}

void bin_swap(pii* a,pii *b)
{
    pii temp=*a;
    *a=*b;
    *b=temp;
}

int bin_maxLeafIndex(int n)
{
    return floor((n/2)-1);
}

void bin_display(myHeap* head)
{
    if(head->size==0){
        printf("Empty\n");
    }
    for(int i=0;i<head->size;i++){
        printf("%d,%d ",head->arr[i].second,head->arr[i].first);
    }
    printf("\n");
}

int bin_percUp(myHeap** head,int index,map<int,int> &mapi)
{
    if(index==0){
        return index;
    }
    while(1){
        if(index==0){
            return index;
        }
        if((*head)->arr[bin_parentIndex(index)].first <= (*head)->arr[index].first){
            return index;
        }
        mapi[(*head)->arr[index].second]=bin_parentIndex(index);
        mapi[(*head)->arr[bin_parentIndex(index)].second]=index;
        bin_swap(&(*head)->arr[bin_parentIndex(index)],&(*head)->arr[index]);
        index=bin_parentIndex(index);
    }
}

void bin_percDown(myHeap** head,int index,map<int,int> &mapi)
{
    while(1){
        if(index>bin_maxLeafIndex((*head)->size) && index>bin_maxLeafIndex((*head)->size)){
            return;
        }
        if(bin_rightChild(index)>=(*head)->size){
            if((*head)->arr[bin_leftChild(index)].first<(*head)->arr[index].first){
                mapi[(*head)->arr[index].second]=bin_leftChild(index);
                mapi[(*head)->arr[bin_leftChild(index)].second]=index;
                bin_swap(&(*head)->arr[index],&(*head)->arr[bin_leftChild(index)]);
            }
            return;
        }
        if((*head)->arr[bin_leftChild(index)].first >= (*head)->arr[index].first &&  (*head)->arr[bin_rightChild(index)].first >= (*head)->arr[index].first){
            return;
        }
        if((*head)->arr[bin_leftChild(index)].first>(*head)->arr[bin_rightChild(index)].first){
            mapi[(*head)->arr[index].second]=bin_rightChild(index);
            mapi[(*head)->arr[bin_rightChild(index)].second]=index;
            bin_swap(&(*head)->arr[index],&(*head)->arr[bin_rightChild(index)]);
            index=bin_rightChild(index);
        }
        else{
            mapi[(*head)->arr[index].second]=bin_leftChild(index);
            mapi[(*head)->arr[bin_leftChild(index)].second]=index;
            bin_swap(&(*head)->arr[index],&(*head)->arr[bin_leftChild(index)]);
            index=bin_leftChild(index);
        }
    }
}


int bin_insert(myHeap** head,pii data,map<int,int> &mapi)
{
    if((*head)->size>=(*head)->cap){
        printf("Full\n");
        return -1;
    }
    (*head)->arr[(*head)->size]=data;
    int index=bin_percUp(head,(*head)->size,mapi);
    (*head)->size++;
    return index;
}

void bin_deletion(myHeap** head,int index,map<int,int> &mapi)
{
    pii temp=(*head)->arr[index];
    (*head)->arr[index]=(*head)->arr[(*head)->size-1];
    mapi[(*head)->arr[(*head)->size-1].second]=index;
    (*head)->size--;
    bin_percDown(head,index,mapi);
}

pii bin_popMin(myHeap** head,map<int,int> &mapi)
{
    pii temp=(*head)->arr[0];
    bin_deletion(head,0,mapi);
    return temp;
}

myHeap* bin_init(int n)
{
    myHeap* head=(myHeap*)malloc(sizeof(myHeap));
    head->cap=n+3;
    head->arr=(pair<int,int>*)malloc(head->cap*sizeof(pair<int,int>));
    head->size=0;
    return head;
}

void bin_dec_key(myHeap** head,int index,int nava,map<int,int> &mapi)
{
    (*head)->arr[index].first=nava;
    bin_percUp(head,index,mapi);
}

//function to relax the edges
int relax(int init,int wt,int source)
{//init is initial weight
    if(init<=source+wt){
        return init;
    }
    else{
        return source+wt;
    }
}

void dijkstra_binary(vector<vector<int>> v,int s,int* bell)
{
    s=s-1;
    int n=v.size();
    int dist[n];
    for(int i=0;i<n;i++){
        dist[i]=MAX;
    }
    bool check[n],inHeap[n];
    for(int i=0;i<n;i++){
        check[i]=false;
    }
    map<int,int>mapi;
    for(int i=0;i<n;i++){
        mapi[i]=-1;
    }
    int cnt=1;
    dist[s]=0;
    int cancel=0;
    bool out_heap[n];
    //priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>> > heap;
    myHeap* heap=bin_init(n);
    for(int i=0;i<n;i++){
        if(i==s){
            continue;
        }
        mapi[i]=bin_insert(&heap,make_pair(dist[i],i),mapi);
    }
    //bin_display(heap);
    for(int i=0;i<n;i++){
        if(v[s][i]!=MAX && i!=s){
            if(v[s][i]<0){
                cancel=1;
                break;
            }
            dist[i]=relax(dist[i],v[s][i],dist[s]);
            bin_dec_key(&heap,mapi[i],dist[i],mapi);
            cnt++;
            inHeap[i]=true;
        }
    }
    check[s]=true;
    while(heap->size!=0){
        int sam=bin_popMin(&heap,mapi).second;
        out_heap[sam]=true;
        for(int i=0;i<n;i++){
            if(v[sam][i]!=MAX && i!=sam){
                if(v[sam][i]<0){
                    cancel=1;
                    break;
                }
                if(!check[i]){
                    //cout<<sam+1<<" "<<i+1<<endl;
                    int hola=relax(dist[i],v[sam][i],dist[sam]);
                    if(hola==dist[i]){
                        continue;
                    }
                    dist[i]=hola;
                    //heap.push(make_pair(dist[i],i));
                    bin_dec_key(&heap,mapi[i],dist[i],mapi);
                    //inHeap[i]=true;
                }
            }
        }
        if(cancel==1){
            break;
        }
        check[sam]=true;
    }
    if(cancel==1){
    	free(heap->arr);
    	free(heap);
        cout<<-1<<endl;
        return;
    }
    for(int i=0;i<n;i++){
        if(dist[i]==MAX){
            cout<<dist[i]<<" ";
            continue;
        }
        cout<<dist[i]-bell[s+1]+bell[i+1]<<" ";
    }
    cout<<endl;
   	free(heap->arr);
   	free(heap);
}

void dijkstra_array(vector<vector<int>> v,int s,int* bell)
{
    s=s-1;
    int n=v.size();
    int dist[n];
    for(int i=0;i<n;i++){
        dist[i]=MAX;
    }
    bool check[n];
    for(int i=0;i<n;i++){
        check[i]=false;
    }
    dist[s]=0;
    int cancel=0;
    for(int i=0;i<n;i++){
        if(v[s][i]!=MAX && i!=s){
            if(v[s][i]<0){
                cancel=1;
                break;
            }
            dist[i]=relax(dist[i],v[s][i],dist[s]);
        }
    }
    check[s]=true;
    int fl=n;
    while(fl--){
        int mini=MAX;
        int sam;
        int ww=0;
        for(int i=0;i<n;i++){
            if(dist[i]<mini && !check[i]){
                mini=dist[i];
                sam=i;
                ww=1;
            }
        }
        if(ww==0){
            for(int i=0;i<n;i++){
                if(!check[i]){
                    ww=1;
                    sam=i;
                    break;
                }
            }
            if(ww==0){
                break;
            }
        }
        for(int i=0;i<n;i++){
            if(v[sam][i]!=MAX && i!=sam){
                if(v[sam][i]<0){
                    cancel=1;
                    break;
                }
                if(!check[i]){
                    int hola=relax(dist[i],v[sam][i],dist[sam]);
                    if(hola==dist[i]){
                        continue;
                    }
                    dist[i]=hola;
                }
            }
        }

        if(cancel==1){
            break;
        }
        check[sam]=true;
    }
    if(cancel==1){
        cout<<-1<<endl;
        return;
    }
    for(int i=0;i<n;i++){
        if(dist[i]==MAX){
            cout<<dist[i]<<" ";
            continue;
        }
        cout<<dist[i]-bell[s+1]+bell[i+1]<<" ";
    }
    cout<<endl;
}

int* belman(vector<vector<int>> v,int s)
{
    int n=v.size();
    int* dist;
    dist=(int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++){
        dist[i]=MAX;
    }
    s=s-1;
    dist[s]=0;
    bool check[n];
    for(int i=0;i<n;i++){
        check[i]=false;
    }
    int sam=s;
    queue<int> qu;
    qu.push(s);
    check[s]=true;
    int cnt=0;
    int total=0;
    int used =0;
    int ch=0;
    map<int,int> mapi;
    while(1){
        if(qu.empty()){
            ch=1;
            break;
        }
        sam=qu.front();
        qu.pop();
        check[sam]=false;
        int ww=0;
        int hola;
        int qq=0;
        for(int j=0;j<n;j++){
            if(v[sam][j]!=MAX && sam!=j){
                //cout<<sam+1<<" "<<j+1<<endl;
                int sample=relax(dist[j],v[sam][j],dist[sam]);
                total++;
                if(sample!=dist[j]){
                    ww=1;
                    used++;
                    if(!check[j]){
                        qu.push(j);
                        check[j]=true;
                    }
                    mapi[j]++;
                    if(mapi[j]==n){
                        qq=1;
                        break;
                    }
                }
                dist[j]=sample;
            }
        }
        if(qq==1){
        	free(dist);
            //cout<<-1<<endl;
            int* arrrr=(int*)malloc(2*sizeof(int));
            arrrr[0]=-1;
            return arrrr;
        }
    }
    return dist;
}

typedef struct node1{
    pii val;
    int degree;
    struct node1 *parent;
    vector<struct node1*> child;
    struct node1 *next;
}mialHeap;

mialHeap* mial_newnode(pii data)
{
    mialHeap* temp=(mialHeap*)calloc(1,sizeof(mialHeap));
    temp->val=data;
    temp->parent=NULL;
    temp->next=NULL;
    vector<mialHeap*> v;
    temp->child=v;
    temp->degree=0;
    return temp;
}

mialHeap* mial_merge(mialHeap* h1,mialHeap* h2)
{
    mialHeap* head=(mialHeap*)calloc(1,sizeof(mialHeap));
    head->next=NULL;
	vector<mialHeap*> v;
    head->child=v;
    mialHeap* freeh1=h1;
    mialHeap* freeh2=h2;

    mialHeap* temp=head;
    h1=h1->next;
    h2=h2->next;

    while(1){
        if(h1==NULL && h2==NULL){
            break;
        }
        if(h1==NULL){

            temp->next=h2;
            temp=temp->next;
            mialHeap* a=h2;
            h2=h2->next;
            a=NULL;
            continue;
        }
        if(h2==NULL){

            temp->next=h1;
            temp=temp->next;
            mialHeap* a=h1;
            h1=h1->next;
            a=NULL;
            continue;
        }
        if(h1->degree>h2->degree){

            temp->next=h2;
            temp=temp->next;
            mialHeap* a=h2;
            h2=h2->next;
            a=NULL;
        }
        else{

            temp->next=h1;
            temp=temp->next;
            mialHeap* a=h1;
            h1=h1->next;
            a=NULL;
        }
    }//merging finished
    //heap to work on now ----> head
    free(freeh1);
    free(freeh2);
    return head;
}

mialHeap* mial_union(mialHeap* h1,mialHeap* h2,mialHeap* &mini)
{
    mialHeap* head=mial_merge(h1,h2);

    mialHeap* temp=head;
    temp=temp->next;
    if(temp==NULL){
        return head;
    }
    mialHeap* prev=head;
    if(temp->val.first<mini->val.first){
        mini=temp;
    }
    while(temp->next!=NULL){
        if(temp->val.first<mini->val.first){
            mini=temp;
        }
        if(temp->next->degree!=temp->degree){
            prev=temp;
            temp=temp->next;
            continue;
        }
        if(temp->next->next!=NULL){
            if(temp->next->next->degree==temp->next->degree){
                prev=temp;
                temp=temp->next;
                continue;
            }
        }
        mialHeap* a=temp->next;

        if(temp->val.first<=a->val.first){

            temp->child.push_back(a);
            temp->next=a->next;
            a->next=NULL;
            a->parent=temp;
            temp->degree++;

        }
        else{
            a->child.push_back(temp);
            prev->next=a;
            temp->parent=a;
            temp->next=NULL;
            a->degree++;
            temp=a;
        }
    }
    if(temp->val.first<mini->val.first){
        mini=temp;
    }

    return head;
}

void mial_insert(mialHeap* &head,pii data,mialHeap* & mini,map<int,mialHeap*> &mapi)
{
    if(head->next==NULL){
        head->next=mial_newnode(data);
        mapi[data.second]=head->next;
        mini=head->next;
        return;
    }
    mialHeap* temp=(mialHeap*)calloc(1,sizeof(mialHeap));
    temp->next=mial_newnode(data);
    mapi[data.second]=temp->next;

    head=mial_union(head,temp,mini);


}

mialHeap* mial_extract_min(mialHeap* &head,mialHeap* &mini)
{
    mialHeap* temp=head;
    if(temp->next==NULL){
        return NULL;
    }
    mialHeap* toreturn=mini;
    while(temp->next!=NULL){
        if(temp->next==mini){
            temp->next=mini->next;
            mini->next=NULL;
            break;
        }
        temp=temp->next;
    }
    mialHeap* b=(mialHeap*)calloc(1,sizeof(mialHeap));
    mialHeap* a=b;
    for(int i=0;i<mini->child.size();i++){
        a->next=mini->child[i];
        a=a->next;
        a->parent=NULL;
    }
    mialHeap* sam=mini;
    if(head->next==NULL){
        mini=b->next;
    }
    else{
        mini=head->next;
    }
    sam=NULL;
    head=mial_union(head,b,mini);
    return toreturn;
}

void mial_dec_key(mialHeap* &head,int index,int nava,map<int,mialHeap*> &mapi,mialHeap* &mini)
{
    mialHeap* n=mapi[index];
    n->val.first=nava;
    if(nava<mini->val.first){
        mini=n;
    }
    while(1){
        if(n->parent==NULL){
            break;
        }
        if(n->parent->val.first<=n->val.first){
            break;
        }
        pair<int,int> temp={n->val.first,n->val.second};
        n->val.first=n->parent->val.first;
        n->val.second=n->parent->val.second;
        n->parent->val.first=temp.first;
        n->parent->val.second=temp.second;
        mapi[n->val.second]=n;
        mapi[n->parent->val.second]=n->parent;
        n=n->parent;
    }
    if(nava<mini->val.first){
        mini=n;
    }
}


void dijkstra_binomial(vector<vector<int>> v,int s,int* bell)
{
    s=s-1;
    int n=v.size();
    int dist[n];
    for(int i=0;i<n;i++){
        dist[i]=MAX;
    }
    bool check[n];
    for(int i=0;i<n;i++){
        check[i]=false;
    }
    map<int,mialHeap*>mapi;
    for(int i=0;i<n;i++){
        mapi[i]=NULL;
    }
    dist[s]=0;
    int cancel=0;
    mialHeap* heap=(mialHeap*)calloc(1,sizeof(mialHeap));
    mialHeap* mini=(mialHeap*)calloc(1,sizeof(mialHeap));
    heap->next=NULL;
    mini=NULL;
    for(int i=0;i<n;i++){
        if(i==s){
            continue;
        }

        mial_insert(heap,make_pair(dist[i],i),mini,mapi);
    }
    for(int i=0;i<n;i++){
        if(v[s][i]!=MAX && i!=s){
            if(v[s][i]<0){
                cancel=1;
                break;
            }
            int sample=dist[i];
            dist[i]=relax(dist[i],v[s][i],dist[s]);
            if(sample!=dist[i]){
                mial_dec_key(heap,i,dist[i],mapi,mini);
            }
        }
    }

    check[s]=true;
    //cout<<"hi"<<endl;
    while(1){
        mialHeap* sample=mial_extract_min(heap,mini);
        if(sample==NULL){
            break;
        }
        if(sample->val.first==MAX){
            break;
        }
        int sam=sample->val.second;
        for(int i=0;i<n;i++){
            if(v[sam][i]!=MAX && i!=sam){
                if(v[sam][i]<0){
                    cancel=1;
                    break;
                }
                if(!check[i]){
                    int hola=relax(dist[i],v[sam][i],dist[sam]);
                    if(hola==dist[i]){
                        continue;
                    }
                    dist[i]=hola;
                    mial_dec_key(heap,i,dist[i],mapi,mini);

                    if(mini==NULL){
                        continue;
                    }

                }
            }
        }
        if(cancel==1){
            break;
        }
        check[sam]=true;
    }

    if(cancel==1){
   	    free(heap);
	    free(mini);
    	for(int i=0;i<n;i++){
			if(mapi[i]==NULL || i==s || mapi[i]==heap || mapi[i]==mini){
				continue;
			}
			free(mapi[i]);
	   	}
        cout<<-1<<endl;
        return;
    }
    for(int i=0;i<n;i++){
        if(dist[i]==MAX){
            cout<<dist[i]<<" ";
            continue;
        }
        cout<<dist[i]-bell[s+1]+bell[i+1]<<" ";
        //cout<<dist[i]<<" ";
    }
    cout<<endl;
    free(heap);
    free(mini);
    for(int i=0;i<n;i++){
    	if(mapi[i]==NULL || i==s || mapi[i]==heap || mapi[i]==mini){
    		continue;
    	}
    	free(mapi[i]);
   	}
}


void johnson_binomial(vector<vector<int>> v)
{
    vector<vector<int>> ans;
    int* dist;
    dist=belman(v,1);
    if(dist[0]==-1){
    	free(dist);
        cout<<-1<<endl;
        return;
    }
    int n=v.size();
    for(int i=0;i<n-1;i++){
        vector<int>sample;
        for(int j=0;j<n-1;j++){
            if(v[i+1][j+1]==MAX){
                sample.push_back(MAX);
                continue;
            }
            sample.push_back(v[i+1][j+1]+dist[i+1]-dist[j+1]);
        }
        ans.push_back(sample);
    }
    for(int i=0;i<ans.size();i++){
        dijkstra_binomial(ans,i+1,dist);
    }
    free(dist);
}

void johnson_array(vector<vector<int>> v)
{

    vector<vector<int>> ans;
    int* dist;
    dist=belman(v,1);
    if(dist[0]==-1){
    	free(dist);
        cout<<-1<<endl;
        return;
    }
    int n=v.size();
    for(int i=0;i<n-1;i++){
        vector<int>sample;
        for(int j=0;j<n-1;j++){
            if(v[i+1][j+1]==MAX){
                sample.push_back(MAX);
                continue;
            }
            sample.push_back(v[i+1][j+1]+dist[i+1]-dist[j+1]);
        }
        ans.push_back(sample);
    }
    for(int i=0;i<ans.size();i++){
        dijkstra_array(ans,i+1,dist);
    }
	free(dist);
}

void johnson_binary(vector<vector<int>> v)
{
    vector<vector<int>> ans;
    int* dist;
    dist=belman(v,1);
    if(dist[0]==-1){
    	free(dist);
        cout<<-1<<endl;
        return;
    }
    int n=v.size();
    for(int i=0;i<n-1;i++){
        vector<int>sample;
        for(int j=0;j<n-1;j++){
            if(v[i+1][j+1]==MAX){
                sample.push_back(MAX);
                continue;
            }
            sample.push_back(v[i+1][j+1]+dist[i+1]-dist[j+1]);
        }
        ans.push_back(sample);
    }
    for(int i=0;i<ans.size();i++){
        dijkstra_binary(ans,i+1,dist);
    }
    free(dist);
}

typedef struct node2{
    pii val;
    struct node2* parent;
    struct node2* child;
    struct node2* left;
    struct node2* right;
    int rnk;
    bool mark;
}fibHeap;

fibHeap* newnode(pii data)
{
    fibHeap* root=(fibHeap*)malloc(sizeof(fibHeap));
    root->val=data;
    root->parent=NULL;
    root->child=NULL;
    root->left=root;
    root->right=root;
    root->mark=false;
    root->rnk=0;
    return root;
}


fibHeap* fib_new_min(fibHeap* &root,fibHeap* &mini)
{
    fibHeap* head=root;
    mini=head;
    head=head->right;
    while(head!=root){
        if(head->val.first<mini->val.first){
            mini=head;
        }
        head=head->right;
    }
    return mini;
}

fibHeap* fib_union(fibHeap* h1,fibHeap* h2)
{
    if(h1->val.first<=h2->val.first){
        h2->left->right=h2->right;
        h2->right->left=h2->left;
        h2->right=NULL;
        h2->left=NULL;
        if(h1->child!=NULL){
            h2->parent=h1;
            h2->right=h1->child;
            h2->left=h1->child->left;
            h1->child->left->right=h2;
            h1->child->left=h2;
        }
        else{
            h1->child=h2;
            h2->parent=h1;
            h2->left=h2;
            h2->right=h2;
        }
        h1->rnk++;
        return h1;
    }
    else{
        h1->left->right=h1->right;
        h1->right->left=h1->left;
        h1->left=NULL;
        h1->right=NULL;
        if(h2->child!=NULL){
            h1->parent=h2;
            h1->right=h2->child;
            h1->left=h2->child->left;
            h2->child->left->right=h1;
            h2->child->left=h1;
        }
        else{
            h2->child=h1;
            h1->parent=h2;
            h1->left=h1;
            h1->right=h1;
        }
        h2->rnk++;
        return h2;
    }
}

fibHeap* consolidate(fibHeap* &root)
{
    if(root==NULL){
        return NULL;
    }
    map<int,vector<fibHeap*>> hola;
    fibHeap* head=root;
    hola[head->rnk].push_back(head);
    head=head->right;

    while(head!=root){
        int noadd=0;
        for(int i=0;i<hola[head->rnk].size();i++){
            if(hola[head->rnk][i]==head){
                noadd=1;
                break;
            }
        }
        if(noadd==1){
            head=head->right;
            continue;
        }
        hola[head->rnk].push_back(head);
        int temp=head->rnk;
        int ww=1;
        while(ww==1){
            ww=0;
            if(hola[temp].size()>1){
                if(hola[temp][hola[temp].size()-2]==root){
                    head=fib_union(hola[temp][hola[temp].size()-2],hola[temp][hola[temp].size()-1]);
                }
                else{
                    head=fib_union(hola[temp][hola[temp].size()-1],hola[temp][hola[temp].size()-2]);
                }
                hola[temp+1].push_back(head);
                hola[temp].pop_back();
                hola[temp].pop_back();
                ww=1;
                temp++;
            }
        }
        head=head->right;
    }

    return root;
}

fibHeap* fib_extract_min(fibHeap* &root,fibHeap* &mini)
{
    if(mini==NULL){
        return NULL;
    }
    fibHeap* toreturn=mini;
    if(mini->left==mini){
        if(mini->child==NULL){
            mini=NULL;
            root=NULL;
            return toreturn;
        }
        else{
            root=mini->child;
            fibHeap* temp=root;
            temp->parent=NULL;
            temp=temp->right;
            while(temp!=root){
                temp->parent=NULL;
                temp=temp->right;
            }
            mini=root;
            fib_new_min(root,mini);
            root=consolidate(mini);
            fib_new_min(root,mini);
            return toreturn;
        }
    }
    if(mini->child==NULL){
        mini->left->right=mini->right;
        mini->right->left=mini->left;
        root=mini->right;
        fibHeap* temp=mini;
        mini=fib_new_min(root,mini);
        root=consolidate(mini);
        fib_new_min(root,mini);
        temp->right=NULL;
        temp->left=NULL;
        temp=NULL;
        return toreturn;
    }
    fibHeap* myChild=mini->child;
    fibHeap* sample=myChild;
    sample->parent=NULL;
    sample=sample->right;
    while(sample!=myChild){
        sample->parent=NULL;
        sample=sample->right;
    }
    mini->left->right=myChild;
    fibHeap* a=myChild->left;
    myChild->left=mini->left;
    a->right=mini->right;
    mini->right->left=a;
    fibHeap* temp=mini;
    temp=NULL;
    root=myChild;
    fib_new_min(root,mini);
    root=consolidate(mini);
    fib_new_min(root,mini);
    return toreturn;
}

void fib_insert(fibHeap* &root,pii data,fibHeap* &mini,map<int,fibHeap*> &mapi)
{
    if(root==NULL){
        root=newnode(data);
        mapi[data.second]=root;
        mini=root;
        return;
    }
    fibHeap* temp=newnode(data);
    mapi[data.second]=temp;
    fibHeap* a=root->left;
    temp->left=root->left;
    temp->right=root;
    root->left=temp;
    a->right=temp;
    root=temp;
    if(temp->val<mini->val){
        mini=temp;
    }
}

void fib_decrease_key(fibHeap* &root,int vertex,map<int,fibHeap*> &mapi,int nava,fibHeap* &mini)
{
    fibHeap* head=root;
    fibHeap* n=mapi[vertex];

    if(n==NULL){
        return;
    }
    n->val.first=nava;
    if(mini->val.first>nava){
        mini=n;
    }
    if(n->parent==NULL){
        return;
    }
    if(nava>=n->parent->val.first){
        return;
    }
    while(1){
        if(n==NULL){
            break;
        }
        if(n->parent==NULL){
            break;
        }
        if(n->parent->mark==false){
            n->parent->mark=true;
            if(n->parent->parent==NULL){
                n->parent->mark=false;
            }
            if(n->parent->child==n){
                if(n->parent->rnk==1){
                    n->parent->child=NULL;
                    n->parent->rnk=0;
                    n->right=head;
                    n->left=head->left;
                    n->parent=NULL;
                    head->left->right=n;
                    head->left=n;
                }
                else{
                    n->parent->child=n->right;
                    n->right->left=n->left;
                    n->left->right=n->right;
                    n->right=head;
                    n->left=head->left;
                    head->left->right=n;
                    head->left=n;
                    n->parent->rnk--;
                    n->parent=NULL;
                }
            }
            else{
                n->parent->rnk--;
                n->right->left=n->left;
                n->left->right=n->right;
                n->right=head;
                n->left=head->left;
                head->left->right=n;
                head->left=n;
                n->parent=NULL;

            }
            n->mark=false;
            break;
        }
        else{
            if(n->parent->child==n){
                if(n->parent->rnk==1){
                    n->parent->child=NULL;
                    n->parent->rnk=0;
                    n->right=head;
                    n->left=head->left;
                    head->left->right=n;
                    head->left=n;
                }
                else{
                    n->parent->child=n->right;
                    n->right->left=n->left;
                    n->left->right=n->right;
                    n->right=head;
                    n->left=head->left;
                    head->left->right=n;
                    head->left=n;
                    n->parent->rnk--;
                }
            }
            else{
                n->parent->rnk--;
                n->right->left=n->left;
                n->left->right=n->right;
                n->right=head;
                n->left=head->left;
                head->left->right=n;
                head->left=n;
            }
            n->mark=false;
            fibHeap* temp=n;
            n=n->parent;
            temp->parent=NULL;
        }
    }
}


void dijkstra_fibo(vector<vector<int>> v,int s,int* bell)
{
    s=s-1;
    int n=v.size();
    int dist[n];
    for(int i=0;i<n;i++){
        dist[i]=MAX;
    }
    bool check[n],inHeap[n];
    for(int i=0;i<n;i++){
        check[i]=false;
    }
    map<int,fibHeap*>mapi;
    for(int i=0;i<n;i++){
        mapi[i]=NULL;
    }
    dist[s]=0;
    int cancel=0;
    fibHeap* heap=(fibHeap*)malloc(sizeof(fibHeap));
    fibHeap* mini=(fibHeap*)malloc(sizeof(fibHeap));
    fibHeap* freeheap=heap;
    fibHeap* freemini=mini;
    heap=NULL;
    mini=NULL;
    for(int i=0;i<n;i++){
        if(i==s){
            continue;
        }
        fib_insert(heap,{dist[i],i},mini,mapi);
    }
    for(int i=0;i<n;i++){
        if(v[s][i]!=MAX && i!=s){
            if(v[s][i]<0){
                cancel=1;
                break;
            }
            int sample=dist[i];
            dist[i]=relax(dist[i],v[s][i],dist[s]);
            if(sample!=dist[i]){
                fib_decrease_key(heap,i,mapi,dist[i],mini);
            }
        }
    }
    check[s]=true;
    while(1){
        fibHeap* sample=fib_extract_min(heap,mini);
        if(sample==NULL){
            break;
        }
        if(sample->val.first==MAX){
            break;
        }
        fibHeap* hello=sample;
        int sam=sample->val.second;
        for(int i=0;i<n;i++){
            if(v[sam][i]!=MAX && i!=sam){
                if(v[sam][i]<0){
                    cancel=1;
                    break;
                }
                if(!check[i]){
                    int hola=relax(dist[i],v[sam][i],dist[sam]);
                    if(hola==dist[i]){
                        continue;
                    }
                    dist[i]=hola;
                    fib_decrease_key(heap,i,mapi,dist[i],mini);
                }
            }
        }
        if(cancel==1){
            break;
        }
        check[sam]=true;
    }
    if(cancel==1){
	    free(freeheap);
    	free(freemini);
	    for(int i=0;i<n;i++){
			if(mapi[i]==NULL || i==s || mapi[i]==heap || mapi[i]==mini){
				continue;
			}
			free(mapi[i]);
	    }
        cout<<-1<<endl;
        return;
    }
    for(int i=0;i<n;i++){
        if(dist[i]==MAX){
            cout<<dist[i]<<" ";
            continue;
        }
        cout<<dist[i]-bell[s+1]+bell[i+1]<<" ";
    }
    cout<<endl;
    free(freeheap);
    free(freemini);
    for(int i=0;i<n;i++){
    	if(mapi[i]==NULL || i==s || mapi[i]==heap || mapi[i]==mini){
    		continue;
    	}
        free(mapi[i]);
    }
}

void johnson_fibo(vector<vector<int>> v)
{
    vector<vector<int>> ans;
    int* dist;
    dist=belman(v,1);
    if(dist[0]==-1){
    	free(dist);
        cout<<-1<<endl;
        return;
    }
    int n=v.size();
    for(int i=0;i<n-1;i++){
        vector<int>sample;
        for(int j=0;j<n-1;j++){
            if(v[i+1][j+1]==MAX){
                sample.push_back(MAX);
                continue;
            }
            sample.push_back(v[i+1][j+1]+dist[i+1]-dist[j+1]);
        }
        ans.push_back(sample);
    }
    for(int i=0;i<ans.size();i++){
        dijkstra_fibo(ans,i+1,dist);
    }
    free(dist);
}

int main(int argc,char* argv[])
{
    int t;
    cin>>t;
    vector<double> tim_array;
    vector<double> tim_binary;
    vector<double> tim_bino;
    vector<double> tim_fibo;
    int sample=0;
    int ww=0;
    if(argc!=1){
    	sample=atoi(argv[1]);
    }
    else{
    	ww=1;
    }
    while(t--){
        int n,d;
        cin>>n>>d;
        n++;
        vector<vector<int>> v;
        vector<int>sam(n);
        for(int i=0;i<n;i++){
            sam[i]=0;
        }
        v.push_back(sam);
        for(int i=1;i<n;i++){
            vector<int> sam(n);
            sam[0]=MAX;
            for(int j=1;j<n;j++){
                cin>>sam[j];
            }
            v.push_back(sam);
        }
        if(sample==1 && ww==0){
            clock_t starti=clock();
            johnson_array(v);
            clock_t endi=clock();
            tim_array.push_back((double)(endi-starti)/CLOCKS_PER_SEC);
        }
        else if(sample==2 && ww==0){
            clock_t starti=clock();
            johnson_binary(v);
            clock_t endi=clock();
            tim_binary.push_back((double)(endi-starti)/CLOCKS_PER_SEC);
        }
        else if(sample==3 && ww==0){
            clock_t starti=clock();
            johnson_binomial(v);
            clock_t endi=clock();
            tim_bino.push_back((double)(endi-starti)/CLOCKS_PER_SEC);
        }
        else{
            clock_t starti=clock();
            johnson_fibo(v);
            clock_t endi=clock();
            tim_fibo.push_back((double)(endi-starti)/CLOCKS_PER_SEC);
        }
    }
    if(sample==1 && ww==0){
        for(int i=0;i<tim_array.size();i++){
            cout<<fixed<<tim_array[i]<<setprecision(6)<<" ";
        }
        cout<<endl;
    }
    else if(sample==2 && ww==0){
        for(int i=0;i<tim_binary.size();i++){
            cout<<fixed<<tim_binary[i]<<setprecision(6)<<" ";
        }
        cout<<endl;
    }
    else if(sample==3 && ww==0){
        for(int i=0;i<tim_bino.size();i++){
            cout<<fixed<<tim_bino[i]<<setprecision(6)<<" ";
        }
        cout<<endl;
    }
    else{
        for(int i=0;i<tim_fibo.size();i++){
            cout<<fixed<<tim_fibo[i]<<setprecision(6)<<" ";
        }
        cout<<endl;
    }
}
