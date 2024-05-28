#include <stdio.h>
int main () {
	// estrutura 
	struct ponto{
		int x;
		int y;  
	}p1, p2, p3, p4;

	//scan valores
	scanf("%d %d", &p1.x, &p1.y); 
	scanf("%d %d", &p2.x, &p2.y); 
	scanf("%d %d", &p3.x, &p3.y); 
	scanf("%d %d", &p4.x, &p4.y); 

	//teste de inclusao
	if ((p2.x - p1.x) >= (p4.x - p3.x)) {
		if ((p2.y - p1.y) >= (p4.y - p3.y))
			printf("o segundo retangulo esta contido no primeiro\n");
        else 
	    	printf("o segundo retangulo nao esta contido no primeiro\n");
	}
	else 
		printf("o segundo retangulo nao esta contido no primeiro\n");

return 0;
}