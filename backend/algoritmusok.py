import sys #use of arguments
from itertools import product #for the permutations
from copy import deepcopy #copy objects
from xlwt import Workbook #for test results into excel
import time #to get running time
from itertools import permutations #for the matrix permutations
from itertools import chain  #for the matrix permutations


#class consiting of a number and a list
#list is a parent vector, number is the fuction value
#makes it easier to return one object
class to_return:
  def __init__(self, maxprob, bestpar):
    self.maxprob = maxprob
    self.bestpar = bestpar
  def __str__(self):
    return "The probability: " + str(self.maxprob) + " The parents: " + str(self.bestpar)

##############################
#PROBABILITY CALCULATOR ALGORITHMS
##############################

#simple case likelihood function calculator
#with given directed tree returns the number we want to maximise and whether the tree is valid
#input: list of numbers and list of parents
#returns -1 if not valid tree
def count_prob(vec, par):
    if((len(vec)-1)!=len(par)): #number vector and parent vector should match
        return("Not appropriate length")
    prob=1
    good=True
    for i in range(len(par)):
        x=vec[par[i]]
        for j in range(i):
            if(par[j]==par[i]):
                x=x-vec[j+1] #calculate value of parent minus its former children
        prob=prob*x
        good=(good and (x-vec[i+1])>=0)
    if(not good):
        x=to_return(-1, []) #if once the value is negative the given tree is invalid
        return x
    x=to_return(prob, par)
    return x

#general case likelihood function calculator
#Right way: we use the former row only for the first newborn
#this is the most used way
def general_count_prob_official(matrix, par): 
    for i in matrix:
        if(count_prob(i,par).maxprob==-1):
            return to_return(-1,[])
    newclones=[]
    for i in matrix:
        newc=0
        for j in i:
            if(j!=0):
                newc=newc+1
        newclones.append(newc) #how many clones in each row
    for i in range(len(newclones)-1,0,-1):
        newclones[i]=newclones[i]-newclones[i-1] #how many clones new in a given row
    k=0
    l=0
    lowertriangle=[]
    for i in range(len(newclones)):
        for j in range(newclones[i]):
            lowertriangle.append(matrix[k][0:l+1])
            l=l+1
        k=k+1
    for i in range(len(lowertriangle)):
        for j in range(len(lowertriangle)):
            if(j>i):
                lowertriangle[i].append(0)
    prob=1
    for i in range(1,len(lowertriangle)):
        actprob=lowertriangle[i-1][par[i-1]]
        for j in range(i-1):
            if(par[j]==par[i-1]):
                actprob=actprob-lowertriangle[i-1][j+1]
        prob=prob*actprob
    y=to_return(prob, par)
    return y


#general case likelihood function calculator opt. 2
#Alternate way1: the actual row only if the parent is a newborn
def general_count_prob_opt2(matrix, par):
    for i in matrix:
        if(count_prob(i,par).maxprob==-1):
            return to_return(-1,[])
    prob=1
    newclones=[]
    for i in matrix:
        newc=0
        for j in i:
            if(j!=0):
                newc=newc+1
        newclones.append(newc) #how many clones in each row
    for i in range(len(newclones)-1,0,-1):
        newclones[i]=newclones[i]-newclones[i-1] #how many clones new in a given row
    k=1
    h=1
    for i in range(1,len(newclones)):
        for j in range(newclones[i]):
            if(matrix[k-1][par[h-1]]!=0):
                x=matrix[k-1][par[h-1]]
                for ii in range(h-1):
                    if(par[ii]==par[h-1]):
                        x=x-matrix[k-1][ii+1]
            if(matrix[k-1][par[h-1]]==0):
                x=matrix[k][par[h-1]]
                for ii in range(h-1):
                    if(par[ii]==par[h-1]):
                        x=x-matrix[k][ii+1]
            prob=prob*x
            h=h+1
        k=k+1
    y=to_return(prob, par)
    return y

#general case likelihood function calculator opt 3.
#Alternate way2: always actual row
def general_count_prob_opt3(matrix, par):
    for i in matrix:
        if(count_prob(i,par).maxprob==-1): #if any row not valid return -1
            return to_return(-1,[])
    prob=1
    newclones=[]
    for i in matrix:
        newc=0
        for j in i:
            if(j!=0):
                newc=newc+1
        newclones.append(newc) #how many clones in each row (non-zero values)
    for i in range(len(newclones)-1,0,-1):
        newclones[i]=newclones[i]-newclones[i-1] #how many clones new in a given row
    k=1
    h=1
    for i in range(1,len(newclones)):
        for j in range(newclones[i]):
            x=matrix[k][par[h-1]] #if this is not zero...(modify to one row upper) if zero then from same row
            for ii in range(h-1):
                if(par[ii]==par[h-1]):
                    x=x-matrix[k][ii+1]
            prob=prob*x
            h=h+1
        k=k+1
    y=to_return(prob, par)
    return y

##############################
#READ IN FROM FILE ALGORITHMS
##############################

#simple case read in lines from file
#comvert the text to float
#add funder as the first element of each row
def simple_readin(inputfile):
    cases=[]
    with open(inputfile, 'r') as f: #read in lines
        for line in f:
            if ((len(line) > 0) and ("#" not in line)):
                cases.append(line.split())
    for i in range(len(cases)):
        for j in range(len(cases[i])):
            cases[i][j]=float(cases[i][j]) #convert the elements into numbers
    for i in range(len(cases)):
        cases[i].insert(0, 1.0)
    return cases

#general case read in matrices from file
#comvert the text to float
#add funder as the first element of the row
def general_readin(inputfile):
    matrices=[]
    matrix=[]
    with open(inputfile, 'r') as f: #read in lines
        for line in f:
            if ((len(line) > 0) and ("#" not in line)):
                matrix.append(line.split())
            if("#" in line):
                if(len(matrix)!=0): #if not two separators consecutively
                    matrices.append(matrix)
                for row in matrix:
                    if len(row)!=len(matrix[0]):
                        sys.exit("Invalid input")
                matrix=[]
        if(len(matrix)!=0):
            matrices.append(matrix) #if no separator at the end
    for i in range(len(matrices)): #on every matrix
        for j in range(len(matrices[i])): #on every row
            for k in range(len(matrices[i][j])): #on every item
                matrices[i][j][k]=float(matrices[i][j][k]) #convert
    for i in range(len(matrices)): #create founder
        founderrow=[0.0]*(len(matrices[i][0])+1)
        founderrow[0]=1.0
        matrices[i].insert(0, founderrow)
        for j in range(1, len(matrices[i])):
            matrices[i][j].insert(0, 1.0)
    return matrices

##############################
#SIMPLE CASE ALGORITHMS
##############################

#siple case, finding best tree
#with given numbers, this calls count_prob with every possible ancestry pattern
#actually this is the brute-force algorithm
def check_all_poss(vec):
    x=to_return(-1, []) 
    options=list()
    for i in range(len(vec)-1):
        options.append(i)
    for par in product(options, repeat=len(vec)-1): #all combinations
        good=True
        for i in range(len(par)): #we only need where first one is 0, second is 0 or 1, third is 0, 1 or 2...
            if(par[i]>i):
                good=False
        if(good):
            y=count_prob(vec, par)
            if(y.maxprob>x.maxprob):
                x=y
    x.bestpar=list(x.bestpar)
    return x


#simple case precheck in the given order
#returns the parent options
#loops until there is modification
def line_pre_check(vec, par): 
    for i in range(len(par)):
        if(par[i]!=-1):
            vec[par[i]]=vec[par[i]]-vec[i+1]
    modified=True
    while(modified):
        parentoptions=list()
        modified=False
        for i in range(1,len(vec)):
            if(par[i-1]!=-1):
                parentoptions.append([par[i-1]])
            if(par[i-1]==-1):
                options=0
                parent=-1
                paropt=list()
                for j in range(i):
                    if(vec[j]>=vec[i]):
                        options=options+1
                        parent=j
                        paropt.append(j)
                parentoptions.append(paropt)
                if(options==0):
                    return -1
                if(options==1):
                    par[i-1]=parent
                    vec[parent]=vec[parent]-vec[i]
                    modified=True
    return parentoptions


#recursive algorithm to check all trees smartly simple case
#inputs: original vector, modified vector, partially filled parent vector, sorted indices
#returns highest function value with best tree
def simple_recursive_alg(origvec, vec, par, indsort): #UPDATED
    maxiprob=to_return(-2,[])
    for i in range(1,len(indsort)):
        if(par[indsort[i]-1]!=-1):
            continue
        else:
            maxiprob=to_return(-1,[])
            for j in range(i):
                if((indsort[j]<indsort[i]) and (vec[indsort[j]]>=vec[indsort[i]])):
                    par2=list(par)
                    vec2=list(vec)
                    par2[indsort[i]-1]=indsort[j]
                    vec2[indsort[j]]=vec2[indsort[j]]-vec2[indsort[i]]
                    x=simple_recursive_alg(origvec, vec2, par2, indsort)
                    if(x.maxprob>maxiprob.maxprob):
                        maxiprob.maxprob=x.maxprob
                        maxiprob.bestpar=x.bestpar
            break
    if(maxiprob.maxprob==-2): #not changed to minus 1, it means the parent vector is fully filled
        maxiprob=count_prob(origvec, par)
    return maxiprob


#greedy algorithm in simple case
#without backtrack
#always chooses the locally best option
def simple_greedy(vec):
    par=[-1]*(len(vec)-1)
    for i in range(1, len(vec)):
        best=0
        bestind=-1
        for j in range(0, i):
            this=vec[j]
            for k in range(j+1, i):
                if(par[k-1]==j):
                    this=this-vec[k]
            if(this>best):
                best=this
                bestind=j
        if(best<vec[i]):
            x=to_return(-1, [])
            return x
        else:
            par[i-1]=bestind
    return count_prob(vec, par)


#algorithm makes polinomial guess in simple case
#if returns true then there has to be a valid tree
#if returns false there still might be a valid tree
def guess_solvable(vec): 
    nums=list()
    ubound=0
    for i in vec:
        if(i>ubound):
            ubound=i
    ubound=ubound+1 #number which is larger than every given number
    for i in vec:
        mingreater=ubound
        if(len(nums)!=0):    
            index=0
            for j in range(len(nums)):
                if(nums[j]>=i and nums[j]<mingreater):
                    mingreater=nums[j]
                    index=j
            if(mingreater<ubound):
                nums[index]=nums[index]-i #selecting the smallest number that is still bigger to be the parent
            else:
                return False
        nums.append(i)
    return True


##############################
#GENERAL CASE ALGORITHMS
##############################

#run precheck on all lines of matrix
#creates list of lists of lists (matrix with list elements)
def matrix_pre_check(matrix, par):
	optionalparents=[]
	matrixcopy=deepcopy(matrix) #should not change original matrix
	for i in matrixcopy:
		lineoptions=line_pre_check(i,par)
        if(lineoptions==-1):
            return -1
        optionalparents.append(lineoptions)
	return optionalparents


#run matrix precheck as long as we get new information from the run
#stop when there is no change
def full_pre_check(matrix, par):
    parcopy=deepcopy(par)
    newpar=[]
    listoflists=matrix_pre_check(matrix,par)
    if(listoflists==-1):
        return -1
    for i in range(len(par)): #merging the different conditions for a given node
        intersect=set(listoflists[0][i])
        for j in listoflists:
            intersect=intersect.intersection(j[i])
        if(len(intersect)==1):
            newpar.append(list(intersect)[0]) #label the parent when only one option
        else:
            newpar.append(-1)
    if(newpar.count(-1)!=parcopy.count(-1)): #if new information call again
        newpar=full_pre_check(matrix,newpar)
    return newpar


#general recursive algorithm used in the general case
#uses simple case recursion on last row, calculates the function value according to the parameter
def general_recursive(origmatrix, vec, par, indsort, algParam): 
    maxiprob=to_return(-2,[])
    for i in range(1,len(indsort)):
        if(par[indsort[i]-1]!=-1):
            continue
        else:
            maxiprob=to_return(-1,[])
            for j in range(i):
                if((indsort[j]<indsort[i]) and (vec[indsort[j]]>=vec[indsort[i]])):
                    par2=list(par)
                    vec2=list(vec)
                    par2[indsort[i]-1]=indsort[j]
                    vec2[indsort[j]]=vec2[indsort[j]]-vec2[indsort[i]]
                    x=general_recursive(origmatrix, vec2, par2, indsort, algParam)
                    if(x.maxprob>maxiprob.maxprob):
                        maxiprob.maxprob=x.maxprob
                        maxiprob.bestpar=x.bestpar
            break
    if(maxiprob.maxprob==-2): #not changed to minus 1, it means the parent vector is fully filled
        if algParam=="1":
            maxiprob=general_count_prob_official(origmatrix, par)
        elif algParam=="2":
            maxiprob=general_count_prob_opt2(origmatrix, par)
        else:
            maxiprob=general_count_prob_opt3(origmatrix, par)
    return maxiprob


#get the number of new clones in each row
#find out if unordered
def general_newclones(matrix):
    newclones=[]
    for i in matrix:
        newc=0
        for j in i:
            if(j!=0):
                newc=newc+1
        newclones.append(newc) #how many clones in each row
    for i in range(len(newclones)-1,0,-1):
        newclones[i]=newclones[i]-newclones[i-1] #how many clones new in a given row
    return newclones

#creates all the possible orders in an unordered case 
def general_permutations_upd(vec):
    i=0
    topermutate=[]
    for j in vec:
        a=[]
        for k in range(j):
            a.append(i)
            i=i+1
        topermutate.append(a)
    p=map(permutations, topermutate)
    prod=product(*p)
    return prod


def general_solver_really(inputfile, opt1, opt2, opt3, algParam):
    matrices=general_readin(inputfile) #read in from txt
    counter=2
    for mymatrix in matrices:
        start_time = time.time()
        myvec=general_newclones(mymatrix) #to see the order between them
        myprod=general_permutations_upd(myvec) #the possible permutations
        mytoreturn=to_return(-1,[]) #this is the value to beat (means no solution)
        for i in myprod:
            myorderedmatrix=[]
            mytranspose=list(zip(*mymatrix)) #transpose the matrix
            for j in list(chain.from_iterable(i)):
                myorderedmatrix.append(mytranspose[j])
            myorderedmatrix=list(zip(*myorderedmatrix))
            for j in range(len(myorderedmatrix)):
                myorderedmatrix[j]=list(myorderedmatrix[j])
            mymatrixcopy=deepcopy(myorderedmatrix) #later we will need this
            parents=[-1]*(len(myorderedmatrix[0])-1)
            forsure=full_pre_check(myorderedmatrix, parents)
            if(forsure==-1):
                continue
            indsort=sorted(range(len(mymatrixcopy[len(mymatrix)-1])), key=lambda k: mymatrixcopy[len(mymatrix)-1][k], reverse=True)
            vecrightnow=deepcopy(mymatrixcopy[len(mymatrixcopy)-1]) #copy last row of the matrix
            for j in range(len(forsure)):
                if(forsure[j]!=-1):
                    vecrightnow[forsure[j]]=vecrightnow[forsure[j]]-vecrightnow[j+1] #subtract what we know for sure
            resultforthis=general_recursive(mymatrixcopy, vecrightnow, forsure, indsort, algParam)
            if(resultforthis.maxprob>mytoreturn.maxprob):
                mytoreturn.maxprob=resultforthis.maxprob
                mytoreturn.bestpar=resultforthis.bestpar
        if opt1:
            sheet1.write(counter, 0, mytoreturn.maxprob) 
        if opt2:
            sheet1.write(counter, 1, str(mytoreturn.bestpar)) 
        if opt3:
            sheet1.write(counter, 2, (time.time() - start_time))
        counter=counter+1


##############################
#QT INTEGRATION
##############################

#check if arguments are integers
#for the simple case use through Qt
def check_args():
	for i in range(2, len(sys.argv)):
		if(not sys.argv[i].isdigit()):
			return False
	return True


#creates a list of numbers from the arguments
#if arguments are numbers, for simple case
def get_numlist():
    if(not check_args()):
        f=open("output.txt", "w")
        f.write("-2")
        f.close()
        sys.exit()
    invec=list()
    for i in range(2, len(sys.argv)):
        invec.append(int(sys.argv[i]))
    return invec


#gets the arguments for general case
def get_advanced_args():
    args=list()
    for i in range(2, len(sys.argv)):
        args.append(sys.argv[i])
    if(len(args)!=10):
        sys.exit("Invalid number of arguments")
    if(args[0] not in ["0", "1", "2", "3"]):
        sys.exit("Invalid type argument")
    for i in range(1, 8):
        if(args[i] not in ["0","1"]):
            sys.exit("Invalid algorithm/target arguments")
    return args


#only if matrix is ordered
#if not ordered precheck cannot be executed
def general_precheck(inputfile, opt2, opt3):
    matrices=general_readin(inputfile) #read in from txt
    counter=2
    for mymatrix in matrices:
        start_time = time.time()
        if(len(mymatrix)==len(mymatrix[0])): #if square matrix
            parents=[-1]*(len(mymatrix[0])-1)
            forsure=full_pre_check(mymatrix, parents)
            if opt2:
                sheet1.write(counter, 5, str(forsure))
            if opt3: 
                sheet1.write(counter, 6, (time.time() - start_time))
            counter=counter+1
        else:
            sheet1.write(counter, 5, str("Nem rendezett!"))


#qt advanced options
def advanced_solver():
    args=get_advanced_args()
    filePath=args[-2]
    precheckrec=(args[1]=="1") #set the options
    precheck=(args[2]=="1")
    naiv=(args[3]=="1")
    greedy=(args[4]=="1")
    functionval=(args[5]=="1")
    parentvec=(args[6]=="1")
    runtime=(args[7]=="1")
    sheet1.write(0, 0, 'Precheck+recursion')
    sheet1.write(1, 0, 'Function Value')
    sheet1.write(1, 1, 'Parent Vector')
    sheet1.write(1, 2, 'Running Time')
    sheet1.write(0, 4, 'Precheck')
    sheet1.write(1, 4, 'Function Value')
    sheet1.write(1, 5, 'Parent Vector')
    sheet1.write(1, 6, 'Running Time')
    sheet1.write(0, 8, 'Naiv')
    sheet1.write(1, 8, 'Function Value')
    sheet1.write(1, 9, 'Parent Vector')
    sheet1.write(1, 10, 'Running Time')
    sheet1.write(0, 12, 'Greedy')
    sheet1.write(1, 12, 'Function Value')
    sheet1.write(1, 13, 'Parent Vector')
    sheet1.write(1, 14, 'Running Time')
    if(args[0]=="0"): #simple
        cases=simple_readin(filePath)
        counter=2
        for line in cases:
            backupline=deepcopy(line)
            backupline2=deepcopy(line)
            if naiv:
                start_time = time.time()
                solution=check_all_poss(line)
                if functionval:
                    sheet1.write(counter, 8, solution.maxprob) 
                if parentvec:
                    sheet1.write(counter, 9, str(solution.bestpar)) 
                if runtime:
                    sheet1.write(counter, 10, (time.time() - start_time))
            if greedy:
                start_time = time.time()
                solution=simple_greedy(line)
                if functionval:
                    sheet1.write(counter, 12, solution.maxprob) 
                if parentvec:
                    sheet1.write(counter, 13, str(solution.bestpar)) 
                if runtime:
                    sheet1.write(counter, 14, (time.time() - start_time))
            if precheck:
                start_time = time.time()
                trypar=[-1]*(len(line)-1)
                if(line_pre_check(line, trypar)==-1):
                    sheet1.write(counter, 4, "-1")
                    return None
                sheet1.write(counter, 5, str(trypar)) 
                if runtime:
                    sheet1.write(counter, 6, (time.time() - start_time)) 
            if precheckrec:
                start_time = time.time()
                indsort=sorted(range(len(backupline)), key=lambda k: backupline[k], reverse=True)
                trypar=[-1]*(len(backupline)-1)
                if(line_pre_check(backupline, trypar)==-1):
                    sheet1.write(counter, 0, "-1")
                    return None
                solution=simple_recursive_alg(backupline2, backupline, trypar, indsort)
                if functionval:
                    sheet1.write(counter, 0, solution.maxprob) 
                if parentvec:
                    sheet1.write(counter, 1, str(solution.bestpar)) 
                if runtime:
                    sheet1.write(counter, 2, (time.time() - start_time))
            counter=counter+1
    else: #general
        if precheckrec:
            general_solver_really(filePath, functionval, parentvec, runtime, args[0])
        if precheck:
            general_precheck(filePath, parentvec, runtime)
    wb.save(args[-1] + '.xls') 


#Qt argument 1
#simple case precheck+recursive
def task_one_precheck_recursive():
    invec=get_numlist()
    indsort=sorted(range(len(invec)), key=lambda k: invec[k], reverse=True)
    originvec=deepcopy(invec)
    trypar=[-1]*(len(invec)-1)
    f=open("output.txt", "w")
    if(line_pre_check(invec, trypar)==-1):
        f.write("-1")
        f.close()
        return None
    solution=simple_recursive_alg(originvec, invec, trypar, indsort)
    for i in solution.bestpar:
        f.write(str(i))
        f.write(" ")
    f.write(str(solution.maxprob))
    f.close()

#Qt argument 2
#simple case precheck
def task_two_precheck():
    invec=get_numlist()
    trypar=[-1]*(len(invec)-1)
    f=open("output.txt", "w")
    if(line_pre_check(invec, trypar)==-1):
        f.write("-1")
        f.close()
        return None
    for i in trypar:
        f.write(str(i))
        f.write(" ")
    f.write("0")
    f.close()


#Qt argument 3
#simple case naiv
def task_three_naiv():
    invec=get_numlist()
    solution=check_all_poss(invec)
    f=open("output.txt", "w")
    for i in solution.bestpar:
        f.write(str(i))
        f.write(" ")
    f.write(str(solution.maxprob))
    f.close()


#Qt argument 4
#simple case greedy
def task_four_greedy():
    invec=get_numlist()
    solution=simple_greedy(invec)
    f=open("output.txt", "w")
    for i in solution.bestpar:
        f.write(str(i))
        f.write(" ")
    f.write(str(solution.maxprob))
    f.close()


#Qt argument 5
#simple case guess
def task_five_guess():
    invec=get_numlist()
    f=open("output.txt", "w")
    f.write(str(guess_solvable(invec)))
    f.close()


#main part of the script
#decides about running based on first argument
if(len(sys.argv)>2):
    if(sys.argv[1]=="0"): #"Halado algoritmusok" section
        wb = Workbook()
        sheet1 = wb.add_sheet('Sheet 1', cell_overwrite_ok=True) 
        advanced_solver()
    elif(sys.argv[1]=="1"): #simple case precheck+recursion algrithms
        task_one_precheck_recursive()
    elif(sys.argv[1]=="2"): #simple case precheck algrithm
        task_two_precheck()
    elif(sys.argv[1]=="3"): #simple case naiv algrithm
        task_three_naiv()
    elif(sys.argv[1]=="4"): #simple case greedy algrithm
        task_four_greedy()
    elif(sys.argv[1]=="5"): #simple case guess algrithm
        task_five_guess()
    else:
        sys.exit("Invalid first argument")
