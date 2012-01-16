#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BS 4
#define MAXLEN 8

void resetuse(int usearray[BS][BS]);
void startboard(char boardin[(BS*BS)+2], char board[BS][BS]);
int findword(char *word, char board[BS][BS], int usearray[BS][BS], int wlen);
int findrest(char *word, char board[BS][BS], int usearray[BS][BS], int wlen, int pos, int x, int y);

int main(int argv, char *argc[]){
	int c,lines=0,maxchars=0,currentlen=0,wlen;
	int i,j,k,l;
	char **dictionary;
	char board[BS][BS];
	char boardin[(BS*BS)+2];
	int bcount=0,getout=0;
	int sa=(int)'a';
	int sz=(int)'z';
	int usearray[BS][BS];
	FILE *fp=NULL;
	if(argv!=2){
		printf("Usage: scramblecheat <dictionary filename>\n");
		return 0;
	}

	if(!(fp=fopen(argc[1],"r"))){
		printf("Could not open %s.\n",argc[1]);
		return 0;
	}

	printf("Finding maximum word length and number of words...");
	/*find the number of lines and maximum string length*/
	while((c=fgetc(fp))!=EOF){
		if((unsigned char)c=='\n'){
			lines++;
			currentlen=0;
			continue;
		}
		currentlen++;
		if(currentlen>maxchars)
			maxchars=currentlen;
	}
	maxchars+=2;

	printf("Done.\n");
	printf("Maximum word length = %i\nNumber of words = %i\n",maxchars-2,lines);
	rewind(fp);

	/*preparing dictionary*/
	printf("Preparing dictionary...");
	if(!(dictionary=(char **)malloc((size_t)(lines*sizeof(char *))))){
		printf("\nFailed to allocate memory for dictionary. Exiting.\n");
		return 0;
	}
	for(i=0;i<lines;i++){
		if(!(dictionary[i]=(char *)malloc((size_t)(maxchars*sizeof(char))))){
			for(j=i-1;j>=0;j--)
				free(dictionary[j]);
			free(dictionary);
			printf("\nFailed to allocate memory for dictionary words. Exiting program.\n");
			fclose(fp);
			return 0;
		}
		if(!fgets(dictionary[i],maxchars,fp)){
			for(j=i;j>=0;j--)
				free(dictionary[j]);
			free(dictionary);
			printf("\nFailed to form dictionary at line %i. Exiting program.\n",i+1);
			fclose(fp);
			return 0;
		}
		/*get rid of newline*/
		for(j=0;j<maxchars-1;j++){
			if(j==maxchars-2)
				dictionary[i][j]='\0';
			if(dictionary[i][j]=='\0')
				break;
			if(dictionary[i][j]=='\n'){
				dictionary[i][j]='\0';
				break;
			}
		}
	}
	fclose(fp);
	printf("Done.\n");

	/*get board*/
	while(1){
		printf("Enter board letters in small caps in the order left to right followed by top to bottom or enter an empty board to quit: ");
		fgets(boardin,(BS*BS)+2,stdin);
		/*get rid of newline, make sure length of string is correct*/
		for(i=0;i<(BS*BS)+2;i++){
			if(i==(BS*BS)+1)
				boardin[i]='\0';
			if(boardin[i]=='\0')
				break;
			if(boardin[i]=='\n'){
				boardin[i]='\0';
				break;
			}
		}
		if((int)strlen(boardin)==0){
			printf("Thanks for using the scramble cheat, you disgusting creature.\n");
			break;
		}
		else if((int)strlen(boardin)!=BS*BS){
			printf("Wrong board size! Please try again! Cheat with intelligence!\n");
			continue;
		}

		for(i=0;i<BS;i++){
			for(j=0;j<BS;j++){
				/*check for invalid chars*/
				if(!((int)boardin[bcount]>=sa && (int)boardin[bcount]<=sz)){
					printf("Invalid characters in board! Try again! Cheat with intelligence!\n");
					getout=1;
					bcount=0;
					break;
				}
				board[i][j]=boardin[bcount];
				bcount++;
			}
			if(getout){
				break;
			}
		}
		bcount=0;
		if(getout){
			getout=0;
			for(i=0;i<BS;i++)
				for(j=0;j<BS;j++)
					board[i][j]='0';
		}

		/*generate words*/
		/*for each word in dictionary, try to find it in the board*/
		for(i=0;i<lines;i++){
			resetuse(usearray);
			wlen = strlen(dictionary[i]);
			if(wlen>MAXLEN)
				continue;
			if(findword(dictionary[i],board,usearray,wlen)){
				for(j=0;j<wlen;j++){
					printf("%c",dictionary[i][j]);
					if(dictionary[i][j]=='q')
						printf("u");
				}
				printf("\n");
			}
		}

	}

	/*cleaning up. always clean up after a good job*/
	for(i=0;i<lines;i++)
		free(dictionary[i]);
	free(dictionary);


	return 0;
}

/* reset usearray */
void resetuse(int usearray[BS][BS]){
	int i,j;
	for(i=0;i<BS;i++)
		for(j=0;j<BS;j++)
			usearray[i][j]=0;
}

/*tries to find specified word on board*/
int findword(char *word, char board[BS][BS], int usearray[BS][BS], int wlen){
	int i,j,found=0;

	for(i=0;i<BS;i++){
		for(j=0;j<BS;j++){
			if(board[i][j]==word[0]){
				usearray[i][j]=1;
				if((found=findrest(word,board,usearray,wlen,1,i,j)))
					return found;
				usearray[i][j]=0;
			}
		}
	}
	return found;
}

/*tries to find the rest of the word*/
int findrest(char *word, char board[BS][BS], int usearray[BS][BS], int wlen, int pos, int x, int y){
	int found=0;
	/*word found*/
	if(pos==wlen)
		return 1;
	
	/*try x-1,y-1*/
	if(x-1>=0 && y-1>=0){
		if(!usearray[x-1][y-1] && board[x-1][y-1]==word[pos]){
			usearray[x-1][y-1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x-1,y-1)))
				return found;
			usearray[x-1][y-1]=0;
		}
	}
	/*try x,y-1*/
	if(y-1>=0){
		if(!usearray[x][y-1] && board[x][y-1]==word[pos]){
			usearray[x][y-1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x,y-1)))
				return found;
			usearray[x][y-1]=0;
		}
	}
	/*try x+1,y-1*/
	if(x+1<BS && y-1>=0){
		if(!usearray[x+1][y-1] && board[x+1][y-1]==word[pos]){
			usearray[x+1][y-1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x+1,y-1)))
				return found;
			usearray[x+1][y-1]=0;
		}
	}
	/*try x-1,y*/
	if(x-1>=0){
		if(!usearray[x-1][y] && board[x-1][y]==word[pos]){
			usearray[x-1][y]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x-1,y)))
				return found;
			usearray[x-1][y]=0;
		}
	}
	/*try x+1,y*/
	if(x+1<BS){
		if(!usearray[x+1][y] && board[x+1][y]==word[pos]){
			usearray[x+1][y]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x+1,y)))
				return found;
			usearray[x+1][y]=0;
		}
	}
	/*try x-1,y+1*/
	if(x-1>=0 && y+1<BS){
		if(!usearray[x-1][y+1] && board[x-1][y+1]==word[pos]){
			usearray[x-1][y+1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x-1,y+1)))
				return found;
			usearray[x-1][y+1]=0;
		}
	}
	/*try x,y+1*/
	if(y+1<BS){
		if(!usearray[x][y+1] && board[x][y+1]==word[pos]){
			usearray[x][y+1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x,y+1)))
				return found;
			usearray[x][y+1]=0;
		}
	}
	/*try x+1,y+1*/
	if(x+1<BS && y+1<BS){
		if(!usearray[x+1][y+1] && board[x+1][y+1]==word[pos]){
			usearray[x+1][y+1]=1;
			if((found=findrest(word,board,usearray,wlen,pos+1,x+1,y+1)))
				return found;
			usearray[x+1][y+1]=0;
		}
	}
	return found;
}
