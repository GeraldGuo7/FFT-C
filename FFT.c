#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define N 1024
typedef struct{       //定义一个结构体表示复数的类型
	double real;
	double imag;
}complex;

complex x[N], *W;   //定义输入序列和旋转因子

int size=0;   //定义数据长度
double PI=4.0*atan(1); //定义π 因为tan(π/4)=1 所以arctan（1）*4=π，增加π的精度

void change()
{
	complex temp;
	unsigned short i=0,j=0,k=0;
	double t;
	for(i=0;i<size;i++)
	{
		k=i;
		j=0;
		t=(log(size)/log(2));
		while( (t--)>0 )
		{
			j=j<<1;
			j|=(k & 1);
			k=k>>1;
		}
		if(j>i)
		{
			temp=x[i];
			x[i]=x[j];
			x[j]=temp;
		}
	}
	output();
}
void transform()
{
	int i;
	W=(complex *)malloc(sizeof(complex) * size);
	for(i=0;i<size;i++)
	{
		W[i].real=cos(2*PI/size*i);
		W[i].imag=-1*sin(2*PI/size*i);
	}
}
void add(complex a,complex b,complex *c)
{
	c->real=a.real+b.real;
	c->imag=a.imag+b.imag;
}
void sub(complex a,complex b,complex *c)
{
	c->real=a.real-b.real;
	c->imag=a.imag-b.imag;
}
void mul(complex a,complex b,complex *c)
{
	c->real=a.real*b.real - a.imag*b.imag;
	c->imag=a.real*b.imag + a.imag*b.real;
}
void fft()
{
	int i=0,j=0,k=0,m=0;
	complex q,y,z;
	change();
	for(i=0;i<log(size)/log(2) ;i++)
	{
		m=1<<i;
		for(j=0;j<size;j+=2*m)
		{
			for(k=0;k<m;k++)
			{
				mul(x[k+j+m],W[size*k/2/m],&q);
				add(x[j+k],q,&y);
				sub(x[j+k],q,&z);
				x[j+k]=y;
				x[j+k+m]=z;
			}
		}
	}
}


void output()
{
	int i;
	for(i=0;i<size;i++)
	{	
		printf("%.4f",x[i].real);
		if(x[i].imag>=0.0001)
		{
			printf("+%.4fj\n",x[i].imag);
		}
		else if(fabs(x[i].imag)<0.0001)
		{
			printf("\n");
		}
		else
		{
			printf("%.4fj\n",x[i].imag);
		}
	}
}

int main()
{
	int i;
	printf("输入数据个数\n");
	scanf("%d",&size);//输入数据的长度（2的整数次幂）
	printf("输入数据的实部、虚部\n");
	for(i=0;i<size;i++)
	{
		scanf("%lf%lf",&x[i].real,&x[i].imag);  //输入复数的实部和虚部
	}
	printf("输出倒序后的序列\n");
	transform();//变换序列顺序
	fft();//蝶形运算
	printf("输出FFT后的结果\n");
	output();//输出结果
	return 0;
}
