1 REM This is a fibonacci number calculator!
10 PRINT "How many fibonacci numbers should I calculate?"
20 INPUT X
30 IF X <= 2 THEN GOTO 10
40 PRINT "Calculating the first ",X," fibonacci numbers: "
50 PRINT "fib[0] = 0"
60 PRINT "fib[1] = 1"
70 LET J = 1

100 LET J = J + 1
110 LET A = 0
120 LET B = 1
130 LET I = 1
140 GOSUB 1000
150 PRINT "fib[",J,"] = ",C
160 IF J < X-1 THEN GOTO 100
170 END

1000 LET C = B + A
1010 LET T = B
1020 LET B = C
1030 LET A = T
1040 LET I = I + 1
1050 IF I = J THEN RETURN
1060 GOTO 1000
