/*

   Copyright (c) 2006-2010, The Scripps Research Institute

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Author: Dr. Oleg Trott <ot14@columbia.edu>, 
           The Olson Lab, 
           The Scripps Research Institute

*/

#include "coords.h"

fl rmsd_upper_bound(const vecv& a, const vecv& b) {
	VINA_CHECK(a.size() == b.size());
	fl acc = 0;
	VINA_FOR_IN(i, a) 
		acc += vec_distance_sqr(a[i], b[i]);
	return (a.size() > 0) ? std::sqrt(acc / a.size()) : 0;
}

static fl rmsd_upper_bound_sqr(const vecv& a, const vecv& b) {
	VINA_CHECK(a.size() == b.size());
	fl acc = 0;
	VINA_FOR_IN(i, a)
		acc += vec_distance_sqr(a[i], b[i]);
	return (a.size() > 0) ? (acc / a.size()) : 0;
}

std::pair<sz, fl> find_closest(const vecv& a, const output_container& b) {
	std::pair<sz, fl> tmp(b.size(), max_fl);
	fl best_sqr = max_fl;
	VINA_FOR_IN(i, b) {
		fl res_sqr = rmsd_upper_bound_sqr(a, b[i].coords);
		if(i == 0 || res_sqr < best_sqr) {
			best_sqr = res_sqr;
			tmp.first = i;
		}
	}
	if(tmp.first < b.size())
		tmp.second = std::sqrt(best_sqr);
	return tmp;
}

void add_to_output_container(output_container& out, const output_type& t, fl min_rmsd, sz max_size) {
	if(max_size == 0)
		return;
	if(out.size() >= max_size && !out.empty() && t.e >= out.back().e)
		return; // cannot improve the container

	std::pair<sz, fl> closest_rmsd = find_closest(t.coords, out);
	bool changed = false;
	if(closest_rmsd.first < out.size() && closest_rmsd.second < min_rmsd) { // have a very similar one
		if(t.e < out[closest_rmsd.first].e) { // the new one is better, apparently
			out[closest_rmsd.first] = t; // FIXME? slow
			changed = true;
		}
	}
	else { // nothing similar
		if(out.size() < max_size)
			out.push_back(new output_type(t)); // the last one had the worst energy - replacing 
		else if(!out.empty() && t.e < out.back().e) // FIXME? - just changed
			out.back() = t; // FIXME? slow
		changed = true;
	}
	if(changed)
		out.sort();
}
