5483  SET  $0    4    
5486  SET  $1    1    
5489  CALL 6027 
5491  EQ   $1    $0    6    
5495  JF   $1    5579 

LAB0:
6027  JT   $0    6035  (LAB1)
6030  ADD  $0    $1    1    
6034  RET 

LAB1:
6035  JT   $1    6048  (LAB2)
6038  ADD  $0    $0    32767
6042  SET  $1    $7   
6045  CALL 6027 
6047  RET 

LAB2:
6048  PUSH $0   
6050  ADD  $1    $1    32767
6054  CALL 6027  (LAB0)
6056  SET  $1    $0   
6059  POP  $0    (LAB0)
6061  ADD  $0    $0    32767
6065  CALL 6027  (LAB0)
6067  RET 

