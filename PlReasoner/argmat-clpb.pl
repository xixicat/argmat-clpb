% =======================================================================
% Copyright 2015 Tsinghua University.
% Authors: Fuan Pu (Pu.Fuan@gmail.com)
% 
% Compute Dung's semantics by SWI-Prolog
% Solving Argumentation by Constraint Logic Programming 
% over Boolean variables
% 
% You can copy this code into http://swish.swi-prolog.org/ for online 
% testing. We list some examples to show how to use our program. 
% Note that our program merely for academic reasearch. If you intent to
% other purposes, please let me know.
/** <examples>
?- main(admissible, 1, X), labeling(X).
?- main(complete, 1, X), labeling(X).
?- findall(X, (main(admissible, 1, X), labeling(X)), E).
?- time((main(complete, 1, X), labeling(X))).
?- time((main(complete5, 1, X), labeling(X))).
?- time((X=[1, 0, _, _, _], main(complete, 1, X), labeling(X))).
?- X=[_, 1, _, _, _], main(complete, 1, X), labeling(X).
?- time((findall(X, (X=[1, 0, _, _, _], main(complete5, 1, X), labeling(X)), E))).
?- time((findall(X, (main(complete5, 1, X), labeling(X)), E))).
?- main(stable, 3, X), labeling(X).
?- main(stable2, 3, X), labeling(X).
?- time((aggregate_all(count, (main2(4, X), labeling(X)), Count))).
?- time((aggregate_all(count, (main(admissible, 4, X), labeling(X)), Count))).
?- time((aggregate_all(count, (main(admissible2, 4, X), labeling(X)), Count))).
*/
% =======================================================================

:- use_module(library(clpb)).
:- use_module(library(lists)).
:- use_module(library(clpfd)).

:- set_prolog_stack(global, limit(100 000 000 000)).
% :- set_prolog_flag(clpb_residuals, bdd).

% neg(A, B) :-
%	B = ~A.
neg(A, ~A).

% Boolean product of two variables
% The element-wise product of two Boolean vector
% can be implemented: maplist(bmult, X1, X2, Y).	
% bmult(A,B,C) :-
%	C = A * B.	
bmult(A, B, A*B).

% Boolean sum of two variables
% badd(A,B,C) :-
% 	C = A + B.		
badd(A,B,A+B).

% Boolean exclusive or of two variables
% bxor(A,B,C) :-
% 	C = A#B.
bxor(A, B, A#B).	
	
% Boolean product of three variables	
% bmult3(A,B,C,D) :-
%	D = A * B * C.		
bmult3(A,B,C, A*B*C).

% sum of products of two Boolean vectors
bv_mult(V1, V2, N) :-
	maplist(bmult, V1, V2, P),
	N = +P.   % sum of products	
	
% the multiplication of Boolean matrix and vector.
% Y = Rows * X.
mv_mult([], _, []).
mv_mult([Row | R_tails], X, Y) :-
	bv_mult(Row, X, Y_this),
	append([Y_this], Y_tails, Y),
	mv_mult(R_tails, X, Y_tails).

% The operation *(Exprs) does not work
% We implement it by myself
pi_op([], 1).		
pi_op([L|L_tails], N):-
	pi_op(L_tails, P),
	N = L * P.
	
% The neutrality function
% Y = neg(Rows * X)
neutrality(Rows, X, Y) :-
	mv_mult(Rows, X, Z),
	maplist(neg, Z, Y).
	
% The \neg R^minus(X) = \neg Rows^T * X.
n_r_minus(Rows, X, Y) :-
	transpose(Rows, Ts),
	neutrality(Ts, X, Y).

% The defense function (i.e., the characteristic function).
% Y = F(X) = N(N(X)), N(X) is the neutrality function.
defense(AttM, X, Y) :-
	neutrality(AttM, X, Z),
	neutrality(AttM, Z, Y).

    
% constraints: element-wise equal relation of two lists 
c_eqn(A, B):-
	sat(A =:= B).	

% expression: 	
b_eqn(A, B, C):-
	C = (A =:= B).	
	
% constraint: element-wise less or equal relation of two lists 
c_leq(A, B):-
	sat(A =< B).	

% expression
b_leq(A, B, C):-
	C = (A =< B).		

% constraint: ~(X =< Y)
c_exclusion(X, Y):-
    maplist(b_leq, X, Y, Z),
    pi_op(Z, N), % N = *Z,
	sat(~N).

% The constraint modle [CF1] of conflict freeness.	
conflict_free1(AttM, X) :-
	mv_mult(AttM, X, Y),
	bv_mult(X, Y, N),
	sat(~N).  % sat(N =:= 0). the constraint

% The constraint modle [CF2] of conflict freeness.	
conflict_free2(AttM, X) :-
	neutrality(AttM, X, Y),
    maplist(c_leq, X, Y).  

% The constraint modle [CF3] of conflict freeness.	
conflict_free3(AttM, X) :-
	neutrality(AttM, X, Y),
    maplist(neg, X, NX),
    maplist(badd, NX, Y, Z),
    maplist(sat, Z).

% =======================================================
% The constraint model [ST1] for stable semantics.		
stable1(AttM, X) :-
	neutrality(AttM, X, Y),	
	maplist(c_eqn, X, Y).  % the constraint

% The constraint model [ST2] for stable semantics.	
stable2(AttM, X) :-
	mv_mult(AttM, X, Z),        % Z = Ax
	maplist(bxor, X, Z, Y),     % Y = Ax xor X, note that Ax and Z is not correct
	maplist(sat, Y).    
    
% =======================================================
% The constraint model [AD1] of admissible.			
admissible1(AttM, X) :-
	conflict_free2(AttM, X),
	defense(AttM, X, Y),
	maplist(c_leq, X, Y).  % the constraint
		
% The constraint model [AD2] of admissible.	        
admissible2(AttM, X) :-
	conflict_free2(AttM, X),
    mv_mult(AttM, X, Y),      % Z = Ax
    transpose(AttM, AttM_t),
    mv_mult(AttM_t, X, Z),      % Z = A^T*x
	maplist(c_leq, Z, Y).  % the constraint	Z =< Y

% The constraint model [AD3] of admissible.	    
admissible3(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
	neutrality(AttM, Y, Z),  % Defense(X)
	maplist(bmult, Y, Z, T), % \neg R^+(X) and Defense(X)
	maplist(c_leq, X, T).  % the constraint X =< T 

% The constraint model [AD4] for admissible semantics.	
admissible4(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
    maplist(bmult, X, Y, Z), % Z = X * \neg R^+(X)
	defense(AttM, Z, T),     % T = Defense(Z)
	maplist(c_leq, X, T).  % the constraint X =< T  
    
% The constraint model [AD5] of admissible.		
admissible5(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
	maplist(badd, X, Y, T), % \neg R^+(X) or Defense(X)
    neutrality(AttM, T, Z),  % N(T)
	maplist(neg, X, U),     % maplist(badd, ~X, T, S), must be decomnoneted
	maplist(badd, U, Z, S), % 
	maplist(sat, S).  % the constraint
    
% The constraint model [AD6] of admissible.		
admissible6(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
	maplist(badd, X, Y, T), % \neg R^+(X) or Defense(X)
    mv_mult(AttM, T, Z),  % R^+(T)
    maplist(bmult, X, Z, S), % S = X*T
    sat(+S=:=0).  % the constraint
    
% =======================================================	
% The constraint model [CO1] for complete semantics.	
complete1(AttM, X) :-
	conflict_free2(AttM, X),
	defense(AttM, X, Y),	
	maplist(c_eqn, X, Y).  % the constraint

% The constraint model [CO2] for complete semantics.	
complete2(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
	neutrality(AttM, Y, Z),  % Defense(X)
	maplist(bmult, Y, Z, T), % \neg R^+(X) and Defense(X)
	maplist(c_eqn, X, T).  % the constraint

% The constraint model [CO3] for complete semantics.	
complete3(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
    maplist(bmult, X, Y, Z), % Z = X * \neg R^+(X)
	defense(AttM, Z, T),     % T = Defense(Z)
	maplist(c_eqn, X, T).  % the constraint    

% The constraint model [CO4] for complete semantics.	
complete4(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
    maplist(badd, X, Y, Z),  % Z = X + \neg R^+(X)
	neutrality(AttM, Z, T),  % T = \neg R^+(Z)
	maplist(c_eqn, X, T).  % the constraint    

% The constraint model [CO5] for complete semantics.	
complete5(AttM, X) :-
	neutrality(AttM, X, Y),  % \neg R^+(X)
    maplist(badd, X, Y, Z),  % Z = X + \neg R^+(X)
    mv_mult(AttM, Z, T),     % T = AttM*Z
    maplist(bxor, T, X, R),  % R = T xor X
	maplist(sat, R).         % the constraint

complete5_1(Rows, X):-
	mv_mult(Rows, X, Z),        % Z = Ax
	maplist(neg, Z, Y),         % Y = not Ax
	mv_mult(Rows, Y, Q),        % Q = A(not Ax)
	maplist(badd, Q, Z, R),     % R = A(not Ax) + Ax  
	maplist(bxor, R, X, T),     % T = R xor X
	maplist(sat, T).
 
complete6(Rows, X) :-
	n_r_minus(Rows, X, X1),	 % \neg R^-(X)
	neutrality(Rows, X, X2),  % \neg R^+(X)
	neutrality(Rows, X2, Z),  % Defense(X)
	maplist(bmult, X1, X2, Y1), % _Y1 = \neg R^-(X) and \neg R^+(X)
	% maplist(bmult, Y1, Z, Xu), % _Xu = _Y1 and Defense(X)
	maplist(bmult, X1, Z, Xu), % _Xu = _Y1 and Defense(X)
	maplist(c_leq, S, Xu),  % S is a subset of _Xu
	maplist(badd, X1, X2, Yu),  % _Yu = \neg R^-(X) or \neg R^+(X)
	maplist(c_leq, T, Yu),  % T is a subset of _Yu
	maplist(c_leq, Y1, T),  % _Y1 is a subset of T
	maplist(badd, X, S, V1), % _V1 = X or S
	maplist(bmult, V1, T, V2), % _V2 = _V1 and T
	defense(Rows, V2, V3),
	maplist(c_eqn, X, V3).  % the constraint	

% non-empty sigma-extension
nonempty(Goal, AttM, X) :-
    length(AttM, Len),
	length(X, Len),
	call(Goal, AttM, X),
    sat(+X).  % nonempty constraint
    
% find the extensions that do not include Y
exclusion(Goal, AttM, Y, X) :-
    length(AttM, Len),
	length(X, Len),
    call(Goal, AttM, X),
    c_exclusion(Y, X). % constraint, Y is not included in X.

% fetch all extensions at once.
fetchAll(Goal, AttM, E) :-
    findall(X, (call(Goal, AttM, X), labeling(X)), E).

% fetch all extensions at once given X.
fetchAll(Goal, AttM, X, E) :-
    findall(X, (call(Goal, AttM, X), labeling(X)), E).
	
main(I, X) :- 
	problem(I, Rows),
	length(Rows, Len),
	length(X, Len),
	complete1(Rows, X).


main(Goal, I, X) :-
	problem(I, AttM),
	length(AttM, Len),
	length(X, Len),
	call(Goal, AttM, X).

mainq(Goal, I, X, Y) :-
	problem(I, AttM),
	length(AttM, Len),
	length(X, Len),
	length(Y, Len),
	maplist(b_leq, Y, X, Z),
	%length(Z, Len),
	pi_op(Z, N), %N = *Z,
	sat(~N),
	call(Goal, AttM, X).
	%complete(AttM, X).
	
problem(1, [[0, 0, 0, 0, 0],
            [1, 0, 1, 0, 0],
            [0, 0, 0, 1, 0],
            [0, 0, 1, 0, 0],
            [0, 0, 0, 1, 1]]).

problem(2, [[0, 0, 0],
	        [0, 0, 0],
			[1, 1, 0]]).
			
problem(3, [[0, 1, 1, 1, 0, 0, 1, 0, 1],
[1, 0, 1, 0, 1, 0, 0, 1, 0],
[1, 1, 0, 0, 0, 1, 0, 1, 1],
[1, 0, 0, 0, 1, 1, 1, 0, 0],
[1, 1, 1, 1, 0, 1, 0, 1, 0],
[0, 1, 1, 1, 1, 0, 1, 0, 1],
[1, 0, 0, 1, 1, 0, 0, 1, 1],
[0, 1, 0, 0, 1, 1, 1, 0, 1],
[0, 0, 1, 1, 0, 1, 1, 1, 0]]).
			
problem(4, [[0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
[1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
[0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
[0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0],
[0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
[1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1],
[1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0],
[0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0],
[0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0],
[0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0],
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1],
[1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1],
[0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
[0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0],
[0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0],
[1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1],
[0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0]]).
