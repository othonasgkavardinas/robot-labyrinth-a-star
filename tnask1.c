#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define N 5
#define p 0.7
#define MAX 100

struct node
{
	int i;
	int j;
	int parenti;
	int parentj;
	float f;
};
typedef struct node snode;

int array[N][N];
float apostaseis[N][N];
float apostaseis2[N][N];
int epektaseis = 0;

void nextBool(double pr)
{
	int c,v;
	int p_scaled;
	for(c=0;c<N;c++)
	{
		for(v=0;v<N;v++)
		{
			p_scaled = (int)((rand()%MAX)+1);
	    	if( p_scaled >  pr*((int)MAX+ 1)) 
			{
				array[c][v]=1;
			}
			else
			{
				array[c][v]=0;
			}
		}	
	}
}

void print_array(int array[N][N])
{
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)	
		{
			printf("%d ",array[i][j]);
		}
		printf("\n");
	}
}


void printarray(float array[N][N])
{
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)	
		{
			printf("%f ",array[i][j]);
		}
		printf("\n");
	}
}

float doManhattan(int si,int sj,int gi,int gj)
{
	float distance = 0;
	distance =  (1*abs(gi-si))+((0.5)*abs(gj-sj));
	return distance;
}

float astro(int array[N][N],float ap[N][N],int si,int sj,int fasti,int fastj)
{
	snode *open,*close;
	open = (snode *)malloc((N*N)*sizeof(snode));
	if(open == NULL) exit(1);
	close = (snode *)malloc((N*N)*sizeof(snode));
	if(close == NULL) exit(1);
	float sunoliko_kostos = 0;
	
	int counto=0,countc=0;
	snode Start;
	Start.i = si;
	Start.j = sj;
	Start.parenti = si;
	Start.parentj = sj;
	Start.f = apostaseis[si][sj];
	open[counto] = Start;
	counto++;
	int z,q;
	snode up,down,left,right;
	while(1)
	{
		if(counto==0) break;
		snode v = open[0];
		int thesh = 0;
		int minf = open[0].f;
		if(counto>1)
		{
			for(z=1;z<counto;z++)
			{
				if(open[z].f < minf)
				{
					minf=open[z].f;
					v = open[z];
					thesh = z;
				}
			}
		}
		counto--;
		for(q=0;q<counto;q++)
		{
			if(q>=thesh)
			{
				open[q]=open[q+1];
			}
		}
		open = (snode *)realloc(open,counto*sizeof(snode));
		open = (snode *)realloc(open,(N*N)*sizeof(snode));
		close[countc] = v;
		countc++;
		if(v.i==fasti && v.j==fastj)
		{
			int h;
			for(h=0;h<countc;h++)
			{
				epektaseis += 1;
				//printf("i,j,pi,pj,f %d, %d, %d ,%d ,%f\n",close[h].i,close[h].j,close[h].parenti,close[h].parentj,close[h].f);
			}
			int i,j;
			//printf("end\n");
			int w;
			array[v.parenti][v.parentj] = 7;
			i = v.parenti;
			j = v.parentj;
			sunoliko_kostos += v.f;
			while(1)
			{
				snode found;
				for(w=0;w<countc;w++)
				{
					if(close[w].i == i && close[w].j == j)
					{
						found = close[w];
					}
				}
				i = found.parenti;
				j = found.parentj;
				sunoliko_kostos += found.f;
				if(i==si && j==sj) break;
				array[found.parenti][found.parentj] = 7;
			}
			break;
		}
		else
		{
			if(v.i !=0 && array[(v.i)-1][v.j]!=1)
			{
				int o = 0,cl = 0,k,q;
				snode old;
				int tho,thc;
				up.i = (v.i)-1;
				up.j = (v.j);
				up.parenti = v.i;
				up.parentj = v.j;
				up.f = doManhattan(si,sj,((v.i)-1),(v.j))+ap[(v.i)-1][v.j];
				for(k=0;k<counto;k++)
				{
					if(open[k].i == up.i && open[k].j == up.j)
					{
						old = open[k];
						tho = k;
						o=1;
					}
				}
				for(k=0;k<countc;k++)
				{
					if(close[k].i == up.i && close[k].j == up.j)
					{
						old = close[k];
						thc = k;
						cl=1;
					}
				}
				if(o==0 && cl==0)
				{	
					open[counto]= up;
					counto++;
				}
				else
				{
					if(old.f > up.f)
					{
						if(o==1)
						{
							counto--;
							for(q=0;q<counto;q++)
							{
								if(q>=tho)
								{
									open[q]=open[q+1];
								}
							}
							open = (snode *)realloc(open,counto*sizeof(snode));
							open = (snode *)realloc(open,(N*N)*sizeof(snode));
							open[counto]= up;
							counto++;
						}
						else if(cl==1)
						{
							countc--;
							for(q=0;q<countc;q++)
							{
								if(q>=thc)
								{
									close[q]=close[q+1];
								}
							}
							close = (snode *)realloc(close,countc*sizeof(snode));
							close = (snode *)realloc(close,(N*N)*sizeof(snode));
							open[counto]= up;
							counto++;
						}
					}
				}
			}
			if(v.i != N && array[(v.i)+1][v.j]!=1)
			{
				int o = 0,cl = 0,k,q;
				int tho,thc;
				snode old;
				down.i = (v.i)+1;
				down.j = (v.j);
				down.parenti = v.i;
				down.parentj = v.j;
				down.f = doManhattan(si,sj,((v.i)+1),(v.j))+ap[(v.i)+1][v.j];
				for(k=0;k<counto;k++)
				{
					if(open[k].i == down.i && open[k].j == down.j)
					{
						old = open[k];
						tho = k;
						o=1;
					}
				}
				for(k=0;k<countc;k++)
				{
					if(close[k].i == down.i && close[k].j == down.j)
					{
						old = close[k];
						thc = k;
						cl=1;
					}
				}
				if(o==0 && cl==0)
				{	
					open[counto]= down;
					counto++;
				}
				else
				{
					if(old.f > down.f)
					{
						if(o==1)
						{
							counto--;
							for(q=0;q<counto;q++)
							{
								if(q>=tho)
								{
									open[q]=open[q+1];
								}
							}
							open = (snode *)realloc(open,counto*sizeof(snode));
							open = (snode *)realloc(open,(N*N)*sizeof(snode));
							open[counto]= down;
							counto++;
						}
						else if(cl==1)
						{
							countc--;
							for(q=0;q<countc;q++)
							{
								if(q>=thc)
								{
									close[q]=close[q+1];
								}
							}
							close = (snode *)realloc(close,countc*sizeof(snode));
							close = (snode *)realloc(close,(N*N)*sizeof(snode));
							open[counto]= down;
							counto++;
						}
					}
				}
			}
			if(v.j !=0 && array[v.i][(v.j)-1]!=1)
			{
				int o = 0,cl = 0,k,q;
				int tho,thc;
				snode old;
				left.i = (v.i);
				left.j = (v.j)-1;
				left.parenti = v.i;
				left.parentj = v.j;
				left.f = doManhattan(si,sj,(v.i),((v.j)-1))+ap[v.i][(v.j)-1];
				for(k=0;k<counto;k++)
				{
					if(open[k].i == left.i && open[k].j == left.j)
					{
						old = open[k];
						tho = k;
						o=1;
					}
				}
				for(k=0;k<countc;k++)
				{
					if(close[k].i == left.i && close[k].j == left.j)
					{
						old = close[k];
						thc = k;
						cl=1;
					}
				}
				if(o==0 && cl==0)
				{	
					open[counto]= left;
					counto++;
				}
				else
				{
					if(old.f > left.f)
					{
						if(o==1)
						{
							counto--;
							for(q=0;q<counto;q++)
							{
								if(q>=tho)
								{
									open[q]=open[q+1];
								}
							}
							open = (snode *)realloc(open,counto*sizeof(snode));
							open = (snode *)realloc(open,(N*N)*sizeof(snode));
							open[counto]= left;
							counto++;
						}
						else if(cl==1)
						{
							countc--;
							for(q=0;q<countc;q++)
							{
								if(q>=thc)
								{
									close[q]=close[q+1];
								}
							}
							close = (snode *)realloc(close,countc*sizeof(snode));
							close = (snode *)realloc(close,(N*N)*sizeof(snode));
							open[counto]= left;
							counto++;
						}
					}
				}
			}
			if(v.j != N && array[v.i][(v.j)+1]!=1)
			{
				int o = 0,cl = 0,k,q;
				int tho,thc;
				snode old;
				right.i = (v.i);
				right.j = (v.j)+1;
				right.parenti = v.i;
				right.parentj = v.j;
				right.f = doManhattan(si,sj,(v.i),((v.j)+1))+ap[v.i][(v.j)+1];
				for(k=0;k<counto;k++)
				{
					if(open[k].i == right.i && open[k].j == right.j)
					{
						old = open[k];
						tho = k;
						o=1;
					}
				}
				for(k=0;k<countc;k++)
				{
					if(close[k].i == right.i && close[k].j == right.j)
					{
						old = close[k];
						thc = k;
						cl=1;
					}
				}
				if(o==0 && cl==0)
				{	
					open[counto]= right;
					counto++;
				}
				else
				{
					if(old.f > right.f)
					{
						if(o==1)
						{
							counto--;
							for(q=0;q<counto;q++)
							{
								if(q>=tho)
								{
									open[q]=open[q+1];
								}
							}
							open = (snode *)realloc(open,counto*sizeof(snode));
							open = (snode *)realloc(open,(N*N)*sizeof(snode));
							open[counto]= right;
							counto++;
						}
						else if(cl==1)
						{
							countc--;
							for(q=0;q<countc;q++)
							{
								if(q>=thc)
								{
									close[q]=close[q+1];
								}
							}
							close = (snode *)realloc(close,countc*sizeof(snode));
							close = (snode *)realloc(close,(N*N)*sizeof(snode));
							open[counto]= right;
							counto++;
						}
					}
				}
			}
		}
	}
	return sunoliko_kostos;
}

int main()
{	
	nextBool(p);
	char enter;
	int i,j,k,l;
	int si,sj;
	int xi,xj;
	int yi,yj;
	float sunoliko= 0;
	float kostos1 = 0;
	float kostos2 = 0;

	printf("~~ARRAY~~\n");
	print_array(array);
	printf("\n");
	printf("oi epiloges einai: \n");
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)	
		{
			if(array[i][j]==0)
			{
				printf("(%d,%d) ",i,j);
			}
		}
	}
	printf("\n");
	printf("epelekse arxikh katastash - dhladh suntetagmenes S(i,j) \n");
	printf("i j:");
	scanf("%d%d",&si,&sj);
	scanf("%c",&enter);
	printf("S=(%d,%d)",si,sj);
	printf("\n");
	printf("epelekse stoxo X kati ektos apo (%d,%d) - dhladh suntetagmenes X(i,j) \n",si,sj);
	printf("i j:");
	scanf("%d%d",&xi,&xj);
	scanf("%c",&enter);
	printf("G1=(%d,%d)",xi,xj);
	printf("\n");
	printf("epelekse stoxo Y kati ektos apo (%d,%d),(%d,%d) - dhladh suntetagmenes Y(i,j) \n",si,sj,xi,xj);
	printf("i j:");
	scanf("%d%d",&yi,&yj);
	scanf("%c",&enter);
	printf("Y=(%d,%d)",yi,yj);
	printf("\n");
	printf("ARA EXOYME: S=(%d,%d), X=(%d,%d) , Y=(%d,%d)\n",si,sj,xi,xj,yi,yj);
	printf("\n");
	array[si][sj]=2;
	array[xi][xj]=3;
	array[yi][yj]=4;
	print_array(array);
	printf("\n");
	int fasti=0,fastj=0,lasti=0,lastj=0;
	if(doManhattan(si,sj,xi,xj)<doManhattan(si,sj,yi,yj))
	{
		fasti = xi;
		fastj = xj;
		lasti = yi;
		lastj = yj;
		printf("We will start with X\n");
	}
	else if(doManhattan(si,sj,xi,xj)>doManhattan(si,sj,yi,yj))
	{
		fasti = yi;
		fastj = yj;
		lasti = xi;
		lastj = xj;
		printf("We will start with Y\n");
	}
	else
	{
		printf("It's the same distance, we will start with X\n");
		fasti = xi;
		fastj = xj;
		lasti = yi;
		lastj = yj;
	}
	for(k=0;k<N;k++)
	{
		for(l=0;l<N;l++)
		{
			apostaseis[k][l] = doManhattan(k,l,fasti,fastj);
		}
	}
	printarray(apostaseis);
	kostos1 = astro(array,apostaseis,si,sj,fasti,fastj);
	print_array(array);
	printf("kostos1: %f\n",kostos1);
	printf("epektaseis 1h fora: %d\n",epektaseis);
	for(k=0;k<N;k++)
	{
		for(l=0;l<N;l++)
		{
			apostaseis2[k][l] = doManhattan(k,l,lasti,lastj);
		}
	}
	printarray(apostaseis2);
	kostos2 = astro(array,apostaseis2,fasti,fastj,lasti,lastj);
	print_array(array);
	printf("kostos2: %f\n",kostos2);
	sunoliko = kostos1 + kostos2;
	printf("sunoliko kostos: %f\n",sunoliko);
	printf("sunolikes epektaseis pou eginan: %d\n",epektaseis);
	return 0;
}

