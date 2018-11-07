#include "network.h"
#include <algorithm>

/*! Resizes the list of nodes (\ref values) */
void Network::resize(const size_t &_n) {
	//RandomNumbers
	size_t oldSize = size();
	values.resize(_n); 
 	
	//Remove links if needed
	if (oldSize > size()) {
		for (std::multimap<size_t,size_t>::iterator link = links.begin(); link != links.end(); link++) {
				if( (link->first >= _n) or (link->second >= _n) ) {
					links.erase(link);
				}
		}
	}
	RNG.normal(values) ; // on met des nombres aleatoires 
	
}
    
    
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted
 */
bool Network::add_link(const size_t &_n1, const size_t &_n2) {

	
	
	if(_n1 == _n2) {return false;} // Check if the neighbor is myself
	else if((_n1 >= size()) or (_n2 >= size()) ) {return false;} // Check if indicies out of bounds
	
	//Check if it already exists in both forms (a,b) & (b,a)
	/**std::vector<size_t> nNeigh = neighbors(_n1);
	std::vector<size_t>::iterator it = find (nNeigh.begin(), nNeigh.end(), _n2);
	if (it != nNeigh.end()) {
		return false;
	}*/
	
	for(size_t neigh : neighbors(_n1)){
		
		if (neigh == _n2) return false;
		
		}
    
	links.insert(std::pair<size_t,size_t>(_n1,_n2));
	return true;
}


/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */
size_t Network::random_connect(const double &mean_deg) {
	// Clear all links from map
	links.clear();
	 
	RandomNumbers myRand;
	size_t numOflinkers(0);
	for (unsigned int i=0;i<size();i++) { // For each node.. add links
		size_t x = myRand.poisson(mean_deg); // Get desired x=degree(i) poisson distributed
		int n = degree(i); // Get # of links of i
		
		
		unsigned int numOfNewLinks = (x-n); // Get the # of new links to add
		if (numOfNewLinks >= size()) {
			numOfNewLinks = size()-1;
		}
		
		if (numOfNewLinks > 0) { // There are new links we want to add
			
			//for (unsigned int j=0;j<numOfNewLinks;j++) { // Add new links
				
					while(degree(i) < x){
					std::vector<int> res(1);
					myRand.uniform_int(res,0,size()-1); // Get random neighbor
					add_link(i,res[0]);
					numOflinkers=numOflinkers+1;
					}
				
			//}
		}
	
	}
	
	return numOflinkers; // return number of linkers
}

/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
size_t Network::set_values(const std::vector<double> &newValues) {
	// Get minimum size of two arrays 
	size_t min;
	if (newValues.size() >= size()) {
		min = size();
	} else {
		min = newValues.size();
	}
	
	
	// Copy values
	for (unsigned int i=0;i<min;i++) {
		values[i] = newValues[i];
	}
	
	return min;
}


/*! Number of nodes */
size_t Network::size() const {
		return values.size();			
}


/*! Degree (number of links) of node no *n* */
size_t Network::degree(const size_t &_n) const {
	return neighbors(_n).size();
}


/*! Value of node no *n* */
double Network::value(const size_t &_n) const {
	if (_n >= size()) {
		return 0;
	}
	
	return values[_n]; 
}


/*! All node values in descending order */
std::vector<double> Network::sorted_values() const {
	std::vector<double> orderedValues(values);
	
	std::sort(orderedValues.begin(),orderedValues.end());
	std::reverse(orderedValues.begin(),orderedValues.end());
	return orderedValues;
}

/*! All neighbors (linked) nodes of node no *n* */
std::vector<size_t> Network::neighbors(const size_t &_n) const {
	std::vector<size_t> myNeighbors;
	for(std::pair<size_t,size_t> link : links) {
		if (link.first == _n) {
			myNeighbors.push_back(link.second);	
		} else if (link.second == _n) {
			myNeighbors.push_back(link.first);
		}
	}
	std::sort(myNeighbors.begin(),myNeighbors.end());
	std::reverse(myNeighbors.begin(),myNeighbors.end());
	return myNeighbors;
	
}
