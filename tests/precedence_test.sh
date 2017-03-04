echo A && echo B || echo C && echo D #basic command should output A B D
(echo A && echo B) || (echo C && echo D) #should print A B
((echo A && echo B) || (echo C && echo D)) #should print A B
echo A || echo B && echo C || echo D #should print A C
echo A || (echo B && echo C) || echo D #should print A
(echo A)) #should output syntax error
exit #no infinite loop