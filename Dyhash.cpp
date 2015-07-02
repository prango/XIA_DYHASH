#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#define hashSize 400000
#define initRandom srand(time(&t));
#define Prime 99999999977
#define maxNoElement 100000
time_t t;
long long int noOfElement=0;
struct FirstLevelHashTable{
 long long int a,b,p;
 struct SecondLevelHashTable *ptr[hashSize];
 long long int count[hashSize];
};
struct SecondLevelHashTable{
 long long int a,b,p,length;
 long long int number[0];
};
long long int hashFunction(long long int,long long int,long long int,long long int,long long int); //calculate hash function((ax+b)modp)modm and return corresponding index

void intializeFirstLevelHash(struct FirstLevelHashTable**,long long int[]);//Intialize  memory for first level hashing

void intializeSecondLevelHash(struct FirstLevelHashTable**); //Intialize memory dynamically  for second level hashing corresponding to each first level pointer

void mapKey(struct FirstLevelHashTable**,long long int[]); //Map key values to the hash tables using first & second level hash function and change the hash function(in second level hash) if collision

int rehashAll(struct FirstLevelHashTable**,long long int,long long int[]); //Logic to implement another hash function and rehash if collision in second level hashing

int searchKey(struct FirstLevelHashTable**,long long int); //Search key by the user input:::: return true or false based on key existence in hash

void displayHashValues(struct FirstLevelHashTable**,long long int[]);//display hash key of all the key in seperate file output.out

void main(){
 long long int i=0,j=0;
 clock_t start,end;
 double time_taken;
 long long int number[maxNoElement];
 struct FirstLevelHashTable *head=NULL;
 intializeFirstLevelHash(&head,number);
 intializeSecondLevelHash(&head);
 mapKey(&head,number);
 printf("Hash Table Built Sucessfully!!!!\n");
 displayHashValues(&head,number);
 printf("Output(containing all hash values) has been generated in seperate file(output.out)\n");
 long long int key;
 int result;
 while(1){
  printf("Enter Key to search:::::::::Press -1 to exit\n");
  scanf("%lli",&key);
  if(key==-1)break;
  if(key<0){
   printf("NOT FOUND\n");
   continue;
  }
  start=clock();
  result=searchKey(&head,key);
  end=clock();
  time_taken=((double)(end-start))/CLOCKS_PER_SEC;
  if(result==0){
   printf("Key is present\tTime Taken:%lf sec.\n",time_taken);
  }
  else{
   printf("NOT FOUND\tTime Taken:%lf sec.\n",time_taken);
  }
 }
}

long long int hashFunction(long long int a,long long int b,long long int p,long long int key,long long int mod){
 return (((a*key)+b)%p)%mod;
}

void intializeFirstLevelHash(struct FirstLevelHashTable **head,long long int number[]){
 long long int i;
 *head=(struct FirstLevelHashTable*)malloc(sizeof(struct FirstLevelHashTable));
 initRandom;
 (*head)->p=Prime;
 (*head)->a=rand()%Prime+1;
 (*head)->b=rand()%Prime+1;
 long long int firstIndex;
 for(i=0;i<hashSize;i++){
  (*head)->count[i]=0;
 }
 FILE *fp;
 fp=fopen("input.in","r");
 if(fp==NULL){
  printf("Couldn't open the input file\n");
  exit(-1);
 }
 while(fscanf(fp,"%lli[^\n]",&number[noOfElement])!=EOF){
  firstIndex=hashFunction((*head)->a,(*head)->b,(*head)->p,number[noOfElement],hashSize);
  (*head)->count[firstIndex]++;
  noOfElement++;
  if(noOfElement==maxNoElement){break;}
 }
 fclose(fp);
}

void intializeSecondLevelHash(struct FirstLevelHashTable **head){
 long long int i,j;
 for(i=0;i<hashSize;i++){
  long long int count=(*head)->count[i];
  if(count>=1){
    (*head)->ptr[i]=(struct SecondLevelHashTable*)malloc(sizeof(struct SecondLevelHashTable)+sizeof(long long int)*(count*count));
   (*head)->ptr[i]->length=(count*count);
   long long int a=rand()%Prime+1;
   long long int b=rand()%Prime+1;
   (*head)->ptr[i]->a=a;
   (*head)->ptr[i]->b=b;
   (*head)->ptr[i]->p=Prime;
   for(j=0;j<(count*count);j++){
    (*head)->ptr[i]->number[j]=0;
   }
  }else{
   (*head)->ptr[i]=NULL;
  }
 }
}

void mapKey(struct FirstLevelHashTable **head,long long int number[]){
 long long int firstIndex,i,j,index,value;
 for(i=0;i<noOfElement;i++){
  firstIndex=hashFunction((*head)->a,(*head)->b,(*head)->p,number[i],hashSize);
  if((*head)->count[firstIndex]>=1){
   index=hashFunction((*head)->ptr[firstIndex]->a,(*head)->ptr[firstIndex]->b,(*head)->ptr[firstIndex]->p,number[i],(*head)->ptr[firstIndex]->length);
   value=(*head)->ptr[firstIndex]->number[index];
   if(value!=number[i]){
    if(value>(long long int)0){
     int flag=1;
     long long int arrlength=(*head)->count[firstIndex];
     long long int arr[arrlength],length=((*head))->ptr[firstIndex]->length;
     int count=0;
     for(j=0;j<length;j++){
      if((*head)->ptr[firstIndex]->number[j]>(long long int)0){
       arr[count]=((*head))->ptr[firstIndex]->number[j];
       count++;
      }
      ((*head))->ptr[firstIndex]->number[j]=0;
     }
     arr[count++]=number[i];
     for(j=count;j<arrlength;j++){
      arr[j]=0;
     }
     while(flag==1){
      flag=rehashAll(&(*head),firstIndex,arr);
     }
    }else{
     (*head)->ptr[firstIndex]->number[index]=number[i];
    }
   }
  }
 }

}
int rehashAll(struct FirstLevelHashTable **head,long long int firstIndex,long long int arr[]){
 long long int i=0;
 long long int index,value;
 long long int a=rand()%Prime+1;
 long long int b=rand()%Prime+1;
 (*head)->ptr[firstIndex]->a=a;
 (*head)->ptr[firstIndex]->b=b;
 (*head)->ptr[firstIndex]->p=Prime;
 long long int arrlength=(*head)->count[firstIndex];
 long long int length=(*head)->ptr[firstIndex]->length;
 for(i=0;i<length;i++){
  (*head)->ptr[firstIndex]->number[i]=0;
 }
 for(i=0;i<arrlength;i++){
  index=hashFunction((*head)->ptr[firstIndex]->a,(*head)->ptr[firstIndex]->b,(*head)->ptr[firstIndex]->p,arr[i],length);
  value=(*head)->ptr[firstIndex]->number[index];
  if(value>(long long int)0){
   return 1;
  }else{
   (*head)->ptr[firstIndex]->number[index]=arr[i];
  }
 }
 return 0;
}

int searchKey(struct FirstLevelHashTable **head,long long int key){
 long long int a,b,p,firstIndex,secondIndex;
 a=(*head)->a;
 b=(*head)->b;
 p=(*head)->p;
 firstIndex=hashFunction(a,b,p,key,hashSize);
 if((*head)->ptr[firstIndex]==NULL){
  return 1;
 }
 a=(*head)->ptr[firstIndex]->a;
 b=(*head)->ptr[firstIndex]->b;
 p=(*head)->ptr[firstIndex]->p;
 secondIndex=hashFunction(a,b,p,key,(*head)->ptr[firstIndex]->length);
 if(key==((*head)->ptr[firstIndex]->number[secondIndex])){
  return 0;
 }else{
  return 1;
 }
}
void displayHashValues(struct FirstLevelHashTable **head,long long int number[]){
 long long int i,firstIndex,secondIndex;
 FILE *fout;
 fout=fopen("output.out","w");
 fprintf(fout,"%s\t\t\t%s\t\t%s\n","Key","Outer Index","Inner Index");
 for(i=0;i<noOfElement;i++){
  firstIndex=hashFunction((*head)->a,(*head)->b,(*head)->p,number[i],hashSize);
  if((*head)->ptr[firstIndex]==NULL){
   fprintf(fout,"%lli\t\t%s\n",number[i],"Not Found");
   continue;
  }
  secondIndex=hashFunction((*head)->ptr[firstIndex]->a,(*head)->ptr[firstIndex]->b,(*head)->ptr[firstIndex]->p,number[i],(*head)->ptr[firstIndex]->length);
  fprintf(fout,"%lli\t\t%lli\t\t\t%lli\n",number[i],firstIndex,secondIndex);
 }
 fclose(fout);
}
