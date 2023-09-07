#pragma once
#include <concurrent_unordered_set.h>
#include <unordered_set>
#include <unordered_map>

//start of region
#ifndef ALGO //if not defined
#define ALGO 0000000000 //define
#endif
// end of region


//new region 
#if ALGO > 0 //if greater than zero redefine
#undef ALGO //undefine
#define ALGO 0000000000 //redefine as starting default val
#endif //end
constexpr int ALGval = ALGO; //const equal to macro on comp time
//end of region


//new region
#ifndef ALGOKEY 
#define ALGOKEY ALGO // ALGOKEY defined same as ALGO val
#elif ALGOKEY != ALGO && ALGO > 0
#undef ALGOKEY
#undef ALGO
#define ALGO 0000000000
#define ALGOKEY ALGO
#endif
// end of region

/*
functions as macros before runtime.
*/

//new region
#define KEYSECURE false //pre-set to false

#if KEYSECURE //if key is false
#define CHECK_KEY(KEYSECURE) "key not encrypted" //macro check key holds false prints "".  
#define KEY_SECURITY_STATE KEYSECURE //define key sec state as being bool for false to be retrieved on runtime.
#define SECURE_THE_KEY(x) ((x)*(x)) //simple for now
#undef KEYSECURE 
#define KEYSECURE true
#else //else if keysecure is true
#define KEY_SECURITY_STATE KEYSECURE //define key sec state as being bool for true to be retrieved on runtime.
#define CHECK_KEY(KEYSECURE) "Key is encrypted"
#endif
//end of region 




template <typename Algorithm>  
struct ALGOs {
	bool ListEmpty = false;
	std::unordered_set<Algorithm> AlgorithmList;

	ALGOs() {

	}
	~ALGOs() {

	}
};


template<typename A>
A FindAlgorithm(A Algorithm,A Index) {

	
}




//THIS STRUCT WILL RELATE TO THE MACRO BEING USED.
template<typename T> 
struct ALGOMACRO{

	std::unordered_set<T> Algo_List;
	


	ALGOMACRO() = default;


	~ALGOMACRO() {

	}
};








//will be placed somewhere else
template <typename N> 
struct Key {

	std::unordered_map<N, N> KeysToId;

	struct ID {

		ID() {




		}
		~ID() {

		}
	};
	Key() {

	}
	~Key() {

	}
private: 
	concurrency::concurrent_unordered_set<N> Keys;
	concurrency::concurrent_unordered_multiset<N> KeyDuplicates;
};











