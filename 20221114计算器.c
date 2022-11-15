#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include<memory.h>
#include<stdbool.h>
typedef double stype;
//stack
struct snode//结点
{
	stype data;
	struct snode* next;
};
struct stack//链式栈容器
{
	struct snode* start;
	struct snode* top;
	int length;
};
void SMake(struct stack* s)
{
	s->start = (struct snode*)malloc(sizeof(struct snode));
	s->length = 0;
	s->start->data = 0;
	s->start->next = NULL;
}
bool SPush(struct stack* s, stype data)
{
	struct snode* p = (struct snode*)malloc(sizeof(struct snode));
	p->data = data;
	p->next = s->start->next;
	s->start->next = p;
	s->length++;
	return true;
}
stype SPop(struct stack* s)
{
	if (s->length <= 0)
		return -1;
	struct snode* rubbish = s->start->next;
	s->start->next = rubbish->next;
	stype ret = rubbish->data;
	free(rubbish);
	rubbish = NULL;
	s->length--;
	return ret;
}
stype SRead(struct stack* s)
{
	if (s->length == 0)
		return -1;
	return s->start->next->data;
}
int SSize(struct stack* s)
{
	return s->length;
}
bool SEmpty(struct stack* s)
{
	return s->length == 0;
}
inline bool isequal(double a, double b)
{
	return fabs(a - b) < 0.01;
}
struct stack ator;//operator stack
struct stack nd;//operand stack
void plus()
{
	double right = SPop(&nd);
	double left = SPop(&nd);
	SPush(&nd, left + right);
}
void minus()
{
	double right = SPop(&nd);
	double left = SPop(&nd);
	SPush(&nd, left - right);
}
void times()
{
	double right = SPop(&nd);
	double left = SPop(&nd);
	SPush(&nd, left * right);
}
void multiply()
{
	double right = SPop(&nd);
	double left = SPop(&nd);
	SPush(&nd, left / right);
}
void PushAtor(char ch)
{
	if (ch == '+' || ch == '-')
	{
		while (isequal(SRead(&ator), 1) || isequal(SRead(&ator), 2) || isequal(SRead(&ator), 3) || isequal(SRead(&ator), 4))
		{
			double oper = SPop(&ator);
			if (isequal(oper, 1))
				plus();
			else if (isequal(oper, 2))
				minus();
			else if (isequal(oper, 3))
				times();
			else if (isequal(oper, 4))
				multiply();
		}
		if (ch == '+')
			SPush(&ator, 1);
		else
			SPush(&ator, 2);
	}
	else if (ch == '*' || ch == '/')
	{
		while (isequal(SRead(&ator), 3) || isequal(SRead(&ator), 4))
		{
			double oper = SPop(&ator);
			if (isequal(oper, 3))
				times();
			else if (isequal(oper, 4))
				multiply();
		}
		if (ch == '*')
			SPush(&ator, 3);
		else
			SPush(&ator, 4);
	}
	else if (ch == '(')
		SPush(&ator, 5);
	else if (ch == ')')
	{
		while (1)
		{
			double oper = SPop(&ator);
			if (isequal(oper, 1))
				plus();
			else if (isequal(oper, 2))
				minus();
			else if (isequal(oper, 3))
				times();
			else if (isequal(oper, 4))
				multiply();
			else if (isequal(oper, 5))
				break;
		}
	}
}
double clearStack()
{
	while (!isequal(SRead(&ator), -1))
	{
		double oper = SPop(&ator);
		if (isequal(oper, 1))
			plus();
		else if (isequal(oper, 2))
			minus();
		else if (isequal(oper, 3))
			times();
		else if (isequal(oper, 4))
			multiply();
	}
	return SPop(&nd);
}
int main()
{
	char inp[1024];
	SMake(&ator);
	SMake(&nd);
	memset(inp, 0, 1024);
	gets_s(inp, 1023);
	double num = 0;
	int dot = 0;//判断是否有小数点,及控制小数点后位数
	int las = 0;//判断上次处理的是符号（1）还是数字（0）
	for (int i = 0; ; i++)
	{
		if (inp[i] >= '0' && inp[i] <= '9' && dot == 0)
		{
			num *= 10;
			num += inp[i] - 48;
			las = 0;
		}
		else if (inp[i] == '.')
		{
			dot = 1;
		}
		else if (inp[i] >= '0' && inp[i] <= '9' && dot != 0)
		{
			num += (inp[i] - 48) * pow(10.0, -dot);
			dot++;
			las = 0;
		}
		else if (inp[i] == 0)
		{
			if(las==0)
				SPush(&nd, num);
			num = 0;
			printf("->%lf\n", clearStack());
			break;
		}
		else
		{
			if (las == 0)
			{
				SPush(&nd, num);
				num = 0;
				dot = 0;
			}
			PushAtor(inp[i]);
			las = 1;
		}
	}
	system("pause");
	return 0;
}