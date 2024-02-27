#ifndef Matrix_h
#define Matrix_h

#include <vector>
#include <iostream>

template<class T>
class matrix {
  private:
    unsigned rows;  // number of rows
    unsigned cols;  // number of columns
    std::vector<T> elements; // array of elements
       
  public:

      // accessing number of rows and cols       
       const  unsigned & GetRows() const
       {
	 return rows;
       }

  const unsigned & GetCols() const
  {
    return cols;
  }       

  //Accessing the elements       
  
  // indexing elements as in linear algebra textbooks
  T& operator()( unsigned i, unsigned j ) {
    return elements[(i-1)*cols+(j-1)];
  }

  const T& operator()( unsigned i, unsigned j ) const {
    return elements[(i-1)*cols+(j-1)];
  }
  
  T& element(unsigned i, unsigned j) {
    return elements[(i-1)*cols+(j-1)];
  }

  const T& element(unsigned i, unsigned j) const {
    return elements[(i-1)*cols+(j-1)];
  }




  // constructors
  // Constructs the zero matrix
  matrix( const unsigned &  m, const unsigned & n);
  //Constructs from another matrix
  matrix( const matrix<T>& );
  // destructor
  ~matrix();


      // constructors
      // Constructs the zero matrix
      matrix( const unsigned & m, const unsigned & n)
	{
	  rows = m;
	  cols =n;
	  
	  if( rows == 0 || cols == 0 )
	    throw std::range_error("attempt to create a matrix with zero rows or columns");
	  
	  this->elements = std::vector<T>(rows*cols, 0);
	  
	}

      // Copy constructor 
      matrix( const matrix<T>& cp )
	: rows(cp.rows), cols(cp.cols), elements(cp.elements)
	{
	}
      
      
      ~matrix()
	{
	}
      
      
      
      // comparison
      bool operator==( const matrix<T>& B)
      {
	if (cols != B.GetCols() || rows != B.GetRows())
	  return 0;
	
	for (int i=1; i<= rows; i++)
	  for(int j=1; j<= cols; j++)
	    {
	      if ((*this)(i,j) != B(i,j))
		return 0;
	    }
	
	return 1;
      }    
      

      
      // Adds factor * row_i to row_k
      void ElementaryRowOperation( const unsigned & i, const unsigned & k, const T & factor )
      {	
	for (int j =1; j<= cols; j++)
	  {
	    (*this)(k,j) = (*this)(k,j) + factor * (*this)(i,j);
	  }
      }
  }
  
  
  // Adds factor * col_i to col_k
  void ElementaryColOperation( const unsigned & i, const unsigned & k, const T & factor )
  {	
    for (int s =1; s<= rows; s++)
      {
	(*this)(s,k) = (*this)(s,k) + factor * (*this)(s,i);
      }
  }
  
  
  // matrix multiplication
  matrix<T> operator*( const matrix<T>& B) const
  {
    matrix <T> Res(cols,B.GetRows());	 
    
    if( (*this).cols != B.GetRows() )
      {
	std::cerr <<"\n Error in multiplication operator *: sized not compatible";
      }
    else
      {	      	       	  
	for (int i=1; i<= rows; i++)
	  for(int j=1; j<= B.GetCols(); j++)
	    {
	      Res(i,j) =0; 
	      for(int k=1; k<= cols; k++)
		{
		  Res(i,j) = Res(i,j)+ (*this)(i,k)*B(k,j);
		}
	    }	       
	    }
    return Res;
  }
  
  matrix<T>& operator*=( const matrix<T>& M ) {
    return *this = *this * M;
  }
  
};



// implements the constructor 
template<class T>
matrix<T>::matrix( const unsigned & m, const unsigned & n)
{
  rows = m;
  cols =n;
  
  if( rows == 0 || cols == 0 )
    throw std::range_error("attempt to create a matrix with zero rows or columns");

  this->elements = std::vector<T>(rows*cols, 0);

};

template<class T>
matrix<T>::matrix( const matrix<T>& cp )
  : rows(cp.rows), cols(cp.cols), elements(cp.elements)
{
}

template<class T>
matrix<T>::~matrix()
{
}


template<class T> 
std::ostream& operator<<(std::ostream& os,  matrix <T>  A)
{
  for (int i = 1; i <= A.GetRows(); i++)
    {
       os << "[";
      
      for (int j = 1; j <= A.GetCols(); j++)
        {
	  if (j !=1)
	    os <<"\t";
	  
            os  << A(i,j);
        }      
      os << "]";
        os << std::endl;
    }
  return os;
}

#endif
