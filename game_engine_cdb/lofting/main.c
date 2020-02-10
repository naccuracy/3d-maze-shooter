#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f = NULL;
    int i,n;
    f = fopen("C:\\loft_util.csv","wt");
    if(f!=NULL)
    {
        printf("N = ");
        scanf("%d",&n);
        fprintf(f,"%d;\n",n-1);
        for(i=0;i<n-1;i++)
        {
            fprintf(f,"%d;%d;%d\n",i,i+1+n,i+n);
            fprintf(f,"%d;%d;%d\n",i,i+1,i+1+n);
        }
        i=n-1;
        fprintf(f,"%d;%d;%d\n",i,n,i+n);
        fprintf(f,"%d;%d;%d\n",i,0,n);
        fclose(f);
        printf("\nmessage: file C:\\loft_util.csv was created.");
    }
    else printf("\nfopen error: can not create file.");
    return 0;
}
