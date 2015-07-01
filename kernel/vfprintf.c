#include "kernel.h"

void
vfprintf(void (*printer)(char), const char *ctl, void **args) {
	
	char flag;
	for(;*ctl;ctl++){
		
		if(*ctl!='%'){
			printer(*ctl);
		}
		
		else{
			ctl++;
			flag=*ctl;
			switch(flag){
				case 'd':{
						int temp=*(int *)args;
						int a[100];
						if(temp==0)printer('0');
						else{
							if(temp<0)printer('-'),temp=-temp;
							int index=0;

							while(temp!=0){
								a[index]=temp%10;
								index++;
								temp=temp/10;
							}
							int i=index-1;
							for(;i>=0;i--)
								printer((char)('0'+a[i]));
						}
						args++;
						printer(' ');
					}break;
				case 'c':{
						
						printer(*(char*)args);
						args++;
						printer(' ');
						
					}break;
				case 's':{
						char *str=*(char**)args;
						for(;*str;str++)
							printer(*str);
						args++;
						printer(' ');

					}break;
				case 'f':{
						int a,b;
						double num=*(double *)args;
						
						a=(int )num;
						num=num-(double)a;
						printer((char)('0'+a));
						printer('.');
						b=(int)(num*100000000);
						printk("%d",b);			
					}break;
			}
		}
		
	}
}



