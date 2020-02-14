Author: Vikram Gardella

Course: CS 485G

Date Submitted: 4/3/2017


Program Description: This is a shell script I created that allows the
user to initiate and monitor program executions within Unix. The user can
also set variable values (expressed as strings and helpful for things like
directory names or token counts) that can be later used at their leisure.


Program utilities:

	set VARIABLE VALUE - checks to see if what the user specifies
	in "VARIABLE" exists as a variable name yet and if so, it changes
	its value to what the user specifies in place of "VALUE", if not
	the variable is instantantiated and the value is set.

	prompt NEWPROMPT - changes the prompt (initially "nsh> ") to
	whatever the user specifies in place of "NEWPROMPT"

	dir NEWDIRECTORYNAME - changes the current working directory to
	what the user specifies in place of "NEWDIRECTORYNAME"

	procs - lists all of the current processes by PID

	vars - lists all existing variables within shell

	done - exits the program normally

	help - prints out all possible commands and how to use them

	do CMD PARAM* - Uses a variant of "exec()" to execute the command,
	CMD with a list of parameters, PARAM*, if any

	back CMD PARAM* - Does the same as "do" except the process is run
	in the background

	tovar VARIABLE CMD PARAM* - Does the same as "do" except the output
	is stored in the variable that is specified by the user.

Limitations: Not sure if "back" is functioning correctly and "tovar"
successfully redirects info to a file referred to by either a direct
file location or a variable but results in a segmentation fault right
after.

Things my program does well:
	- ignores comments with endpoints indicated with "%"
	- stores, creates, and changes variables at the user's discretion
	- references and utilizes variables for things like stored
	  directory names
	- changes prompt
	- handles errors/failures with executions as well as invalid user
	  input
	- performs commands using "do", "back", or "tovar" such as "vi",
	  "mkdir", "cp", and so on.


What I Learned: This assignment taught me more in depth about fork() and
exec() as well as how to implement them correctly and gave me more insight
into the applications and functions performed by shell scripts.
