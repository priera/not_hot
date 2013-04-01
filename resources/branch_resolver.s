
#This function decides to which branch of a given case statement the program must jump. Also gives an error if there is no according branch
#
#Arguments: 
#	ACC: Result of case top expression
#	T1: Address of case table
#	T2: Number of classes -used as initialization for minor_dist-
#
#Returns:
#	T1: Pointer to first branch of case table
#	T0: Branch where the execution must jump
#Algorithm:
#minor_dist <= A number greater than the maximum depth of the class tree
#ret = -1
#for branch b in branch_table
#	temp <= load_type_of_brach(b)
#	dist <= distance(actual_type, temp);
#	if dist = 0 then
#		ret <= b
#		break
#	else if dist < minor_dist then
#		minor_dist <= dist
#		ret <= b
#	end if
#end for
#assert(ret != -1)
#return ret

_branch_resolver:
	sw		$ra 0($sp) #push RA
	addiu	$sp $sp -4 
	lw		$t6 0($a0) #load expression's dynamic type
	li		$t0 -1		#return value initialization
	lw		$t3 0($t1)	#branches count
	addiu	$t1 $t1 4	#now T1 points to first branch data
	li		$t4 0			#current branch
_branch_main_loop:
	beq	$t4 $t3 _end_branch_main_loop
	mul	$t5 $t4 8
	add	$t5 $t1 $t5
	lw		$t5 0($t5)	#load into t5 current branch type
	jal	_distance_resolver
	beqz	$t5 _distance_equal_to_0
	bgt	$t5 $t2 _continue_branch_main_loop #if dist > minor_dist then continue
	bne	$t7 $0 _continue_branch_main_loop #We must also check that the _distance_resolver found a correct path
	move	$t2 $t5	#minor_dist <= dist
	move	$t0 $t4	#ret <= b
	b _continue_branch_main_loop
_distance_equal_to_0:
	move	$t0 $t4	#ret <= b
	b _end_branch_main_loop	#break
_continue_branch_main_loop:
	addiu	$t4 $t4 1
	b		_branch_main_loop
_end_branch_main_loop:
	li		$t4 -1
	bne	$t4 $t0 _branch_resolver_ok #assert(ret != -1)
	jal	_case_abort
_branch_resolver_ok:
	lw		$ra 4($sp)
	addiu	$sp $sp 4
	jr $ra				#return
#This functions returns the distance between two classes in the class tree
#
#Arguments:
#	T6: classtag, suposed to be minor
#	T5: classtag, suposed to be the greatest
#Return:
#	T5: Distance between the two classes
#	T7: 0 if the function founds a path between the two classes, -1 otherwise
_distance_resolver:
 	sw		$t0 0($sp)
	addiu $sp $sp -4
	sw		$t1 0($sp)
	addiu $sp $sp -4
	sw 	$t2 0($sp)
	addiu	$sp $sp -4
	sw 	$t3 0($sp)
	addiu	$sp $sp -4
	li 	$t1 0		#step counter: initialize to 0
	li		$t7 -1
	bne	$t6 $t5 _distance_resolver_loop_begin #if both classes are 0, distance is 0. Return
	li		$t7 0
	b _distance_resolver_end
_distance_resolver_loop_begin:
	la 	$t8 class_objTab
	li		$t0 -1	#Object class have a -1 as parent's class tag. 
	move	$t2 $t6
	li		$t3 16 #WORD_SIZE * FIELDS_IN_OBJTAB_REGISTER
_distance_resolver_loop:
	mul	$t2 $t2 $t3
	add	$t2 $t8 $t2
	lw		$t2 0($t2) #Loads parent's class tag
	addiu	$t1 $t1 1	#A new step has been done: increase step counter
	bne	$t2 $t5 _distance_resolver_loop_not_equal
	li		$t7 0		#Parent's tag is equal than the argument passed: end loop
	b _distance_resolver_end
_distance_resolver_loop_not_equal:
	bne	$t2 $t0 _distance_resolver_loop
_distance_resolver_end:
	move	$t5 $t1
	lw		$t3 4($sp)
	addiu	$sp $sp 4
	lw		$t2 4($sp)
	addiu	$sp $sp 4
	lw		$t1 4($sp)
	addiu	$sp $sp 4
	lw		$t0 4($sp)
	addiu	$sp $sp 4
	jr $ra				#return