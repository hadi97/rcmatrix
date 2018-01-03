
#include <math.h>
#include <fstream>
using namespace std; ;
class IndexOutOfRange{};
class WrongDim{};
 
class CMatrix{
	private:
	 
	struct rcmatrix;
	rcmatrix* data;
	 
	public:
	class Cref;
	CMatrix();
	CMatrix(const CMatrix& );
	CMatrix(fstream& );
	CMatrix(int , int , double );
	CMatrix(int , int , double , double);
	~CMatrix();
	CMatrix& operator=(const CMatrix& );
	friend ostream & operator << (ostream & , const CMatrix &);
	friend ostream & operator << (ostream & , const CMatrix::Cref&);
	void check ( int , int ) const ;
	friend CMatrix operator* (const CMatrix&, const CMatrix&);
	const double * operator[](int ) const ; 
	CMatrix::Cref operator[](int);
	

};

struct CMatrix::rcmatrix
{
    double** array;
    int rows;
    int cols;
    int n;
 
	rcmatrix(fstream& file) //First constructor
	{
		file >> this->rows; 
		file >> this->cols;
		try
		{
			this->array = new double*[this->rows]; //allocating array 
	    	for(int i=0;i<this->rows;i++)
	    		this->array[i]=new double[this->cols];
		}
		catch(...)
		{
			throw bad_alloc();
		}
		for(int i=0;i<this->rows;i++)
			for(int j=0;j<this->cols;j++)
				file >> this->array[i][j];

		this->n=1;
	}
    
	rcmatrix(int newrows, int newcols, double val) //Second constructor 
	{
    	try
		{
			this->array = new double*[newrows];
    		for(int i=0;i<newrows;i++)
    			this->array[i]=new double[newcols];
		}
    	catch(...)
		{
    		throw bad_alloc();
    	}
                 
    	for(int i=0;i<newrows;i++) {
        	for(int j=0;j<newcols;j++) {
           		if(i==j)
					this->array[i][j]=val;    	
				else 
					this->array[i][j]=0.0;
			}		
		}		
		this->rows=newrows;
    	this->cols=newcols;
    	this->n=1;
    }
 
    rcmatrix(int newrows, int newcols, double val1, double val2) //Third constructor
	{
    	try
    	{
    		array= new double*[newrows];
    		for(int i=0;i<newrows;i++)
    			array[i]=new double[newcols];
    	}
    	catch(...){
    		throw bad_alloc(); 
    	}                 
    	for(int i=0;i<newrows;i++)
    		for(int j=0;j<newcols;j++)
        		if(i==j)
					array[i][j]=val1;
    			else 
					array[i][j]=val2;                 
    	rows=newrows;
    	cols=newcols;
    	n=1;
    };
	rcmatrix(int newrows, int newcols, double** temp) //fourth constructor 
	{
		rows=newrows;
    	cols=newcols;
        n=1;
		for ( int i=0;i<cols;i++)
			for (int j=0;i<rows;j++)
				this->array[i][j]=temp[i][j];
	}
    rcmatrix(rcmatrix &temp) // copy constructor 
	{
    	this->rows=temp.rows;
    	this->cols=temp.cols;
		try
		{
    		array = new double*[rows];
        	for(int i=0;i<rows;i++)
        		this->array[i]=new double[cols];
        }
    	catch(...)
		{
    		throw bad_alloc();
		}
    	for(int i=0;i<rows;i++) {
        	for( int j=0;j<cols;j++) {
            		array[i][j]=temp.array[i][j];                
    		}
		}		
		this->n=1;
	}
 
    ~rcmatrix() //destructor
	{
    	for(int i=0; i<this->rows; i++)
    		delete[] array[i];
    	delete[] array;
    }        
    rcmatrix* det()
	{
    	if(n==1)
            return this;
		rcmatrix* t=new rcmatrix(rows,cols,array);
    	n--;
		return t;
    }

	void assign(int nrows , int ncols ,double** p)
	{
		if(rows!=nrows || cols != ncols) 
		{ 	
			rows=nrows; 
			cols=ncols;		
		}	
		for ( int i=0;i<rows;i++)
    		for(int j=0;j<cols;j++)
				array[i][j]=p[i][j];	
	}
};
 //-------------End of structure---------------------



//--------------------------Cref Class-----------------
//-----------------------------------------------------
class CMatrix::Cref
{
	friend class CMatrix;
	CMatrix& s;
	int i;
	Cref(CMatrix&_s, int _i) : s(_s) , i(_i) {
		cout<<"Constructor 1 Cref CALLed\n\n"<<endl;
	}
	public:
	double& operator[](int index)   
	{		
		cout << "double CMatrix::operator[](int index) const R1111"<<endl;
		return s.data->array[i][index];
	}
};

CMatrix::Cref CMatrix::operator[](int index)    
{
	cout<<"Cref CMatrix::operator[](int) called R2222" <<endl;
	return Cref(*this,index); 		
}

//---------------------------END OF CREF ----------------------------
//------------------------------------------------------------------- 
inline CMatrix::CMatrix() 
{
	data = new rcmatrix(0,0,0.0); //using constructor 2
}
inline CMatrix::CMatrix(fstream& fs)
{
	data = new rcmatrix(fs); //using constructor 1 
}

inline CMatrix::CMatrix(const CMatrix &temp)
{
	if(data->n==1)
		delete data;
	data = new rcmatrix(0,0,0.0);
	data->assign(temp.data->rows,temp.data->cols,temp.data->array);
}

CMatrix::~CMatrix()
{
	if(data->n==1)
		delete data;
}

CMatrix::CMatrix(int newrows,int newcols, double val)
{
    data = new rcmatrix(newrows,newcols,val); //using constructor 2
}
 
CMatrix::CMatrix(int newrows,int newcols, double val1, double val2)
{
	data = new rcmatrix(newrows,newcols,val1,val2); //using constructor 4
} 

CMatrix& CMatrix::operator = (const CMatrix& temp)
{
	
    if( &temp == this ) // assignment to self?
    {
        return *this;  // if so, don't assign, just return self
    }
	if(data->n==1) 
		data->assign(temp.data->rows,temp.data->cols,temp.data->array); 
    
	return *this;      // allow a = b = c; assignment
}

ostream & operator << (ostream & s, const CMatrix & matrix)
{
	s << "[";
	for(int i=0;i<matrix.data->rows;i++)
	{	
		for(int j=0;j<matrix.data->cols;j++)
		{
			s << matrix.data->array[i][j];
			if(((j+1) % matrix.data->cols) == 0 && j!=0 && i!=matrix.data->rows-1)
				s << "\n ";
			
			if(!(i==matrix.data->rows-1 && j==matrix.data->cols-1) && j!=matrix.data->cols-1)
				s << ", ";
		}
	}
	s << "]";
	return s;
}

inline CMatrix operator * (const CMatrix& m1, const CMatrix& m2)
{
         
    if(m1.data->cols != m2.data->rows)
		throw WrongDim();
         
    CMatrix newMatrix(m1.data->rows, m2.data->cols,0.0);
	double var; 
    for(int i=0;i<newMatrix.data->rows;i++)
        for(int j=0;j<newMatrix.data->cols;j++)
        {
        	var=0.0;    
            for(int lol=0;lol<m1.data->cols;lol++)
                var += m1.data->array[i][lol]*m2.data->array[lol][j];    
            newMatrix.data->array[i][j] = var;
        }
         
    return newMatrix;
}

inline void CMatrix::check (int i, int j) const
{
	if(data->rows<=i || data->cols <=j) 
		throw IndexOutOfRange();
}

const double* CMatrix::operator[](int index) const
{
	cout << "const double * CMatrix::operator[](int i) CALLED R3333"<<endl;
	return data->array[index];
} 
