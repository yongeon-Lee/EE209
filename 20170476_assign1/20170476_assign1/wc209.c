/* Name : Yongeon LEE Student ID :20170476 */
/* Assignment #1 A Word Counting Program */
/* wc209.c */
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
enum DFAstate{COMMENT,CHARACTER};
/* 
   main function 
   FUNCTION: get standard input stream from text file 
   main function has three DFA state which consists of  
   COMMENT state, CHARACTER state and Final state.
   ---------------------------------------------------
   The main logic of function is that it gets two 
   character at first and then, function determine which DFA state is. 
   After that, it saves the second character at 'first' variable. 
  'second' variable get new character and same path continues...
   Final state determines the last character b/c if 'second' variable 
   is EOF, 'first' variable does not read from the other DFA state
 */
int main (void){
  enum DFAstate state=COMMENT;
  int cnt_char=0;
  int cnt_line=1;
  int cnt_word=0;
  int into_space=0;
  //state variable that indicate whether it is word(1) or white space(0)
  //if state changes 1->0 , word count +1 
  int current_line=0;
  int first,second;
  /* Get initial state */
  /* Get two character at first and
    define the inital state */
  first=getchar();
  second=getchar();
  /* If input is NULL= Final State*/
  if(first==EOF){
    cnt_line=0;
    printf("%d %d %d\n",cnt_line,cnt_word,cnt_char);
    return 0;
  }
  /* COMMENT state */
  else if(first=='/'&&second=='*'){
    state=COMMENT;
  }
  /* CHARACTER state */
  else{
    cnt_char++;
    if(isspace(first))into_space=0;
    else into_space=1;
    if(first=='\n'){
      cnt_line++;
    }
    // save second character at 'first' value
    first=second;
    state=CHARACTER;
  }
  /*DFA*/
  while((second=getchar())!=EOF){
    switch(state){
      /* DFAstate = COMMENT */
     case COMMENT:
      /* Since it's COMMENT, make into_space to 0 */
      if(into_space==1){
	cnt_word+=into_space;
	into_space=0;
      }
      current_line=cnt_line;
      // current_line saves the line number which comment begins
      first=second;
      second=getchar(); //read two character right after '/*' 
      /* PASS COMMENT  */
      while(first!='*'||second!='/'){
	if(first=='\n'){
	  cnt_line++;
	  cnt_char++;
	}
	first=second;
	second=getchar();
	// save the variable and read one more character
	/* Standard error stream */
	if(second==EOF){
	  fprintf(stderr,"Error: line %d: unterminated comment\n",current_line);
	  return EXIT_FAILURE;
	}
      }//end while
      cnt_char++;// b/c comment is a space
      second=getchar(); // read character right after */
      first=second;
      state=CHARACTER;
      // b/c COMMENT finished, go to character state
      break;
      // end case COMMENT 
      /* DFAstate = CHARACTER */  
    case CHARACTER:
      // if COMMENT continues, go to comment state
      if(first=='/'&&second=='*'){
	state=COMMENT;
	break;
      }
      cnt_char++;
      /* Determine it is whether white space or not.
         if state change 1 to 0 cnt_word++
         else if state do not change, state remains. */
      if(isspace(first)){
	if(into_space==1){
	  cnt_word+=into_space;
	into_space=0;
	}
      }
      else into_space=1;
      /* Counting the number of line*/
      if(first=='\n'){
	cnt_line++;
      }
      first=second;// save the 'second' variable to 'first' variable
      state=CHARACTER;
      break;
      // end case CHARACTER
    }//end switch
  }//end while
  /* DFAstate = Final State */
  if(first!=EOF){
     cnt_char++;
     if(isspace(first)){
       if(into_space==1){
         cnt_word+=into_space;
	 into_space=0;
	}
      }
     else cnt_word++;
     if(first=='\n'){
       cnt_line++;
      }
  }
  /* STANDARD OUTPUT STREAM */
   printf("%d %d %d\n",cnt_line,cnt_word,cnt_char);
  return 0;
}//end main
