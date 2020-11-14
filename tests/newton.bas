10 REM Square Root Calculator using Newton's Method

100 LET X = 1337
400 GOSUB 1000
500 PRINT "The square root of ",X," is approximately ",Y
600 END

1000 REM Calculate the square root of a given Number
1001 REM Expects input to be in X. Output will be in Y.
1002 REM Uses Z as a temporary variable.
1050 LET Y = 0.5 * X
1100 LET Z = Y
1200 LET Y = Y-(((Y*Y)-X)/(2*Y))
1300 IF Z <> Y THEN GOTO 1100
1400 RETURN
