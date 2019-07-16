//set the kth bit in array A:
void  SetBit( int A[],  int k ){

      int i = k/32;        //gives the corresponding index in the array A
      int pos = k%32;      //gives the corresponding bit position in A[i]

      unsigned int flag = 1;   // flag = 0000.....00001

      flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)

      A[i] = A[i] | flag;      // Set the bit at the k-th position in A[i]
}

void  ClearBit( int A[], int k ) {

      A[k/32] &= ~(1 << (k%32));
}

int TestBit( int A[], int k ){

      return ( (A[k/32] & (1 << (k%32) )) != 0 ) ;
}
