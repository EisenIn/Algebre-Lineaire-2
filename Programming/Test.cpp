#include "Matrix.h"
#include "rational.h"



typedef Rational<int,float> rational; 
int main()
{

  matrix<rational> A(4,4), B(4,4),C(4,4);


  // Make  matrices the identity 
  for (int i = 1; i <= 4; i++)
    {
      A(i,i)=1;
      B(i,i)=1;
      C(i,i)=1;
    }


  matrix<rational> D(A); 

  A.ElementaryRowOperation(1,3,rational(3,2));
  B.ElementaryRowOperation(1,3,rational(-3,2));

  A.ElementaryRowOperation(2,3,rational(4,2));
  B.ElementaryRowOperation(2,3,rational(-4,2));


  
  A.ElementaryColOperation(1,3,rational(3,2));
  C.ElementaryColOperation(1,3,rational(-3,2));
  
  A.ElementaryColOperation(2,3,rational(4,2));
  C.ElementaryColOperation(2,3,rational(-4,2));



  std::cout << "\n" << ((B*A*C) == D);
  std::cout << "\n" << ((B*A) == D); 
  
  
    
  
  
  
}
