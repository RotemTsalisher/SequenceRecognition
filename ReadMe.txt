/* --------------------------------------------------- 
 * 8-bit sequence recognition state machine
 * the state machine will recognize the sequence 1010
 * will return an 8 bit vector with 1 
 * in places that the sequence was found,
 * 0 otherwise.
 * READING VECTORS RIGHT_TO_LEFT! {lsb to mbs}
 * example: v = [10110101] is being read as: {1,0,1,0,1,1,0,1}
 * and will result in '1' as the sequence 1010 is found.
   --------------------------------------------------- */