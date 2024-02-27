#include "Matrix.h"
#include "rational.h"
#include <stdlib.h>
typedef Rational<int,float> rational;   
int main()
{

  matrix<rational> A(4,4), R(4,4),L(4,4); 

 
  // Make  matrices the identity 
  for (int i = 1; i <= 4; i++)
    {
      A(i,i)=1;
      R(i,i)=1;
      L(i,i)=1;   
    }

  
  matrix<rational> B(A);

  int k,l,m,n;

  int N_1,D_1,N_2,D_2; 
  
  for (int i=1; i<=5; i++)
    {      

      k = (rand()%4)+1;
      
      l = (rand()%4)+1;
      while (l ==k)
	l = (rand()%4)+1;


      m = (rand()%4)+1;

      n = (rand()%4)+1;
      while (n ==m)
	n = (rand()%4)+1;

      
      N_1 = rand()%10 -20;
      D_1 = rand()%10+1;

      N_2 = rand()%10 -20;
      D_2 = rand()%10+1;
      
      
      A.ElementaryRowOperation(k,l,rational(N_1,D_1));
      L.ElementaryRowOperation(k,l,rational(N_1,D_1)); 

      
      A.ElementaryColOperation(m,n,rational(N_2,D_2));
      R.ElementaryColOperation(m,n,rational(N_2,D_2));
      
    }


  std::cout << "\n " ;
  
  std::cout << "\n A \n" << A;
  std::cout << "\n L*R \n " << L*R; 
      
  std::cout << "\n (A == (L*R):" << (A == (L*R)); 
  std::cout << "\n (A == L):" << (A == L); 
  
  
    
  
  
  
}
