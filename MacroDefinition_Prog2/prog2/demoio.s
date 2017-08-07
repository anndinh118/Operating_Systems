*----------------------------------------------------------------------
* Programmer: Alan Riggins  
* Assignment or Title: I/O Demo for bsvc
* Filename: demoio.s
* Date completed:  September 1, 2016
*----------------------------------------------------------------------
* Problem statement:
*   This program uses iomacs.s, the I/O macros developed at SDSU to be 
*   used with bsvc.  It also uses its own macro file (casemacs.s).  
*   The program is intended to demonstrate the use of the macro 
*   pre-processor, the I/O macros, and the bsvc I/O interface.
*
* Input: character strings typed by the user
*
* Output: the same strings with all letters converted to uppercase.
*
* Error conditions tested: None.
*
* Included files: /home/ma/cs237/bsvc/iomacs.s
*                 /home/ma/cs237/bsvc/casemacs.s
*
* Method and/or pseudocode: 
*	output greeting
*	do {
*		prompt user for input
*		read input line	
*		echo user's line
*		convert line to upper case (macro call ToUpper)
*		output converted line
*		query user about continuing
*	   }
*	while user enters Y|y to continue
*
* References:  none.
*
*----------------------------------------------------------------------
*
        ORG     $0
        DC.L    $3000           * Stack pointer value after a reset
        DC.L    start           * Program counter value after a reset
        ORG     $3000           * Start at location 3000 Hex
*

*
* Register use
*   D0    length of input line
*   D1    user's response to query
*   A1    address of end of line (for inserting 0)
*
#minclude /home/cs/riggins/cssc0601/assn1/redgy/prog1/blah.s
#minclude /home/cs/riggins/cssc0601/assn2/redgy/test.s
fuuu:	macro
start:  linein                 * Initialize (required for I/O)
        lineout stars           * Output heading
        lineout title
        lineout stars
continue:
        lineout skipln
        lineout prompt
        linein  buffer          * Get input
        lea     buffer,A1        
        adda.l  D0,A1           
        clr.b   (A1)            * Insert 0 byte at end of string
        lineout skipln
        lineout msg
        lineout buffer          * Output user's input
        lineout skipln
        lea     buffer,A1
        ToUpper A1              * Convert to uppercase (macro)
        lineout capmsg
        lineout buffer          * Output converted line
        lineout skipln
        lineout query           * Query user about continuing
        linein  buffer
        move.b  buffer,D1
        cmp.b   #'Y',D1
        beq     continue 
        cmp.b   #'y',D1
        beq     continue
        lineout bye

        break                   * Terminate execution
		endm
*
*       Storage declarations

title:  macro    '***   Welcome to the I/O trap/macro demo   ***',0	
stars:  dc.b    '**********************************************',0
prompt: dc.b    '          Please Enter a string . . .',0        
buffer: ds.b    82
msg:    dc.b    'The string you entered was:',0
capmsg: dc.b    'The string in capital letters is:',0
query:  dc.b    'Do you want to enter another string (y/n)?',0
bye:    dc.b    'The program is finished.',0	
skipln: dc.b    0
        endm
blah: