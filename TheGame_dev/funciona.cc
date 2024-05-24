#include "Player.hh"
#include <queue>
#include <map>
#include <list>
#include <iostream>

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME SedrK_v3


struct PLAYER_NAME : public Player
	{

		/**
		* Factory: returns a new instance of this class.
	  	* Do not modify this function.
		*/
		static Player* factory ()
		{
			return new PLAYER_NAME;
		}

		/**
		* Types and attributes for your player can be defined here.
		*/

		class cmpare
		{
		public:
	    	bool operator()(const Position p1, const Position p2) const
			{
		        if (p1.i!=p2.i) return p1.i<p2.i;
		        return p1.j<p2.j;
    		}
		};

		struct soldat
		{
			int id;
			Data dades;
			int rol; // 0 atacant 1 defensor 2 defesor-atacant
			stack<Position> recorregut;
		};

		typedef vector<int> VE;

		static const int DIST_MAX = 1000000;

		map<int, soldat> soldats; // id, soldat
		vector<Post> P; // posts
		VE S; 		//  soldats del jugador
		VE H; 	//	helicopters del jugador
		VE II = {-1, 1, 0, 0,-1,-1, 1, 1 };
		VE JJ = {0, 0, -1, 1, -1, 1, -1, 1};

		bool enemic_aprop(int x, int y, Position &en)
		{
			for(int i=x-1; i<=x+1; ++i)
			{
				for(int j=y-1; j<=y+1; ++j)
				{
					int s = which_soldier(i,j);
					if (s>0 and data(s).player!=me()) 
					{
						en.i=i; en.j=j;
						return true;
					}
				}
			}
			return false;
		}

		bool coord_valida_S(int x, int y)
		{
			return pos_ok(x,y) and not(what(x,y)==MOUNTAIN or what(x,y)==WATER or fire_time(x,y)>0);
		}

		bool found_in_map_S(map<Position, int, cmpare> &coord_visit, int x, int y)
		{
		    bool found = false;
		    map<Position, int, cmpare>::const_iterator it;
		    it = coord_visit.find(Position(x, y));
		    if (it != coord_visit.end()) found = true;
		    return found;
		}

		int distancia_coord_S(map<Position, int, cmpare> &coord_visit, int x, int y)
		{
		    map<Position, int, cmpare>::iterator it;
		    it = coord_visit.find(Position(x, y));
		    return it->second;
		}

		bool BFS_S(Position b, queue< Position > &cola, int &distancia, vector<vector<Position>> &va, int k)
		{
			map<Position, int, cmpare> coord_visit;

			Position aux(cola.front().i,cola.front().j);
	        coord_visit.insert(make_pair(aux, 0));
	        while (not cola.empty())
			{
	            int x = cola.front().i;
	            int y = cola.front().j;
	            cola.pop();

	            if (b.i == x and b.j == y)
				{
	                distancia = distancia_coord_S(coord_visit, x, y);
	                return true;
	            }

	            if (coord_valida_S(x - 1, y) and not found_in_map_S(coord_visit, x - 1, y)) 			// NORD
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x - 1, y), dist + 1));
	                cola.push(Position(x - 1, y));
					va[x-1][y]=Position(x,y);
	            }

	            if (coord_valida_S(x + 1, y) and not found_in_map_S(coord_visit, x + 1, y)) 			// SUD
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x + 1, y), dist + 1));
	                cola.push(Position(x + 1, y));
					va[x+1][y]=Position(x,y);
	            }

	            if (coord_valida_S(x, y - 1 ) and not found_in_map_S(coord_visit, x, y - 1 )) 		// OEST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x, y - 1 ), dist + 1));
	                cola.push(Position(x, y - 1 ));
					va[x][y-1]=Position(x,y);
	            }

	            if (coord_valida_S(x, y + 1) and not found_in_map_S(coord_visit, x, y + 1)) 			// EST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x, y + 1), dist + 1));
	                cola.push(Position(x, y + 1));
					va[x][y+1]=Position(x,y);
	            }

				if (coord_valida_S(x - 1, y -1) and not found_in_map_S(coord_visit, x - 1, y-1)) 		// NORD-OEST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x - 1, y-1), dist + 1));
	                cola.push(Position(x - 1, y-1));
					va[x-1][y-1]=Position(x,y);
	            }

				if (coord_valida_S(x - 1, y +1) and not found_in_map_S(coord_visit, x - 1, y+1))		// NORD-EST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x - 1, y+1), dist + 1));
	                cola.push(Position(x - 1, y+1));
					va[x-1][y+1]=Position(x,y);
	            }

				if (coord_valida_S(x + 1, y -1) and not found_in_map_S(coord_visit, x + 1, y-1)) 	//SUD-OEST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x + 1, y-1), dist + 1));
	                cola.push(Position(x + 1, y-1));
					va[x+1][y-1]=Position(x,y);
	            }

				if (coord_valida_S(x + 1, y +1) and not found_in_map_S(coord_visit, x + 1, y+1)) 	//SUD-EST
				{
	                int dist = distancia_coord_S(coord_visit, x, y);
	                coord_visit.insert(make_pair(Position(x + 1, y+1), dist + 1));
	                cola.push(Position(x + 1, y+1));
					va[x+1][y+1]=Position(x,y);
	            }
	        }

	        return false;
		}

		void contruir_cami_S(stack<Position> &c, vector<vector<Position>> &v, int i, int j, int ii, int jj)
		{
			while(not (i==ii and j==jj))
			{
				//cout << i << ',' << j << " - ";
				c.push(v[i][j]);
				int aux = i;
				i=v[i][j].i;
				j=v[aux][j].j;
			}
			//cout << endl;
			return;
		}

		void go_soldats(int soldat_id)
		{
			// dades del soldat
			soldat s = soldats.find(soldat_id)->second;
			Position p(data(soldat_id).pos.i, data(soldat_id).pos.j);

			// dades de la base objectiu
			Position b;
			Position ba;

			int dist_prop=DIST_MAX;
			int dist;
			if (s.rol==1) // defensor
			{
				if(enemic_aprop(p.i,p.j,ba))
				{
					cout << "Te voy a matah" << endl;
					dist = abs(p.i-ba.i) + abs(p.j-ba.j);
					b = ba;
				}
				else
				{
					for (int pa=0; pa<P.size(); ++pa)
					{
						ba.i = P[pa].pos.i;
						ba.j = P[pa].pos.j;
						dist = abs(p.i-ba.i) + abs(p.j-ba.j);
						if (dist<dist_prop and post_owner(ba.i, ba.j)== me()) // menor distancia i no hi ha ningu
						{
							dist_prop = dist;
							b = ba;
						}
					}
				}
				int distancia;
				queue< Position > cola;
				cola.push(p); // primera posicio
				vector<vector<Position>> vaprop(60, vector<Position>(60));
				if (BFS_S(b, cola, distancia, vaprop, 0))
				{
					if (distancia == DIST_MAX) {cout << " return" << endl;return;}
					if (distancia == 1 and which_soldier(b.i,b.j)==0) command_soldier(soldat_id, b.i, b.j);
					else if (distancia >= 1 and which_soldier(b.i,b.j)>0) s.rol = 2;
					else
					{
						contruir_cami_S(s.recorregut, vaprop, b.i, b.j, p.i, p.j);
						if (not s.recorregut.empty()){s.recorregut.pop(); if (not s.recorregut.empty())command_soldier(soldat_id, s.recorregut.top().i, s.recorregut.top().j);}
					}
				}
			}
			else // s.rol = 0 or 2 // atacante
			{
				if(enemic_aprop(p.i,p.j,ba))
				{
					dist = abs(p.i-ba.i) + abs(p.j-ba.j);
					b = ba;
				}
				else
				{
					for (int pa=0; pa<P.size(); ++pa)
					{
						ba.i = P[pa].pos.i;
						ba.j = P[pa].pos.j;
						dist = abs(p.i-ba.i) + abs(p.j-ba.j);
						if (dist<dist_prop and post_owner(ba.i, ba.j)!= me() ) // menor distancia i no hi ha ningu
						{
							dist_prop = dist;
							b = ba;
						}
					}
				}
				int distancia;
				queue< Position > cola;
				cola.push(p); // primera posicio
				vector<vector<Position>> vaprop(60, vector<Position>(60));
				if (BFS_S(b, cola, distancia, vaprop, 0))
				{
					if (distancia == DIST_MAX) {cout << " return" << endl;return;}
					else if (distancia == 1) command_soldier(soldat_id, b.i, b.j);
					else
					{
						contruir_cami_S(s.recorregut, vaprop, b.i, b.j, p.i, p.j);
						if (not s.recorregut.empty()){s.recorregut.pop(); if (not s.recorregut.empty()) command_soldier(soldat_id, s.recorregut.top().i, s.recorregut.top().j);}
					}
				}
			}

			soldats.find(soldat_id)->second = s;
		}


		bool coord_valida_H(int x, int y, int ori)
		{
			if (not pos_ok(x,y)) return false;
			if (which_helicopter(x,y)>0) return false;

			if (ori == 1) for (int i = -2 ; i < 2; ++i) if (not pos_ok(x+i,y+2) or what(x+i,y+2)==MOUNTAIN) return false;
			else if (ori == 3) for (int i = -2; i < 2; ++i) if (not pos_ok(x+i,y-2) or what(x+i,y-2)==MOUNTAIN) return false;
			else if (ori == 0) for (int i = -2; i < 2; ++i) if (not pos_ok(x+2,y+i) or what(x+2,y+i)==MOUNTAIN) return false;
			else if (ori == 2) for (int i = -2; i < 2; ++i) if (not pos_ok(x-2,y+i) or what(x-2,y+i)==MOUNTAIN) return false;
			else return false;
			return true;
		}

		bool BFS_H(Position b, queue< Position > &cola, int &distancia, vector<vector<Position>> &va)
		{
			map<Position, int, cmpare> coord_visit;
			Position aux(cola.front().i,cola.front().j);
	        coord_visit.insert(make_pair(aux, 0));
	        while (not cola.empty())
			{
	            int x = cola.front().i;
	            int y = cola.front().j;
	            cola.pop();
	            if (b.i == x and b.j == y)
				{
	                distancia = distancia_coord_S(coord_visit, x, y);
	                return true;
	            }
				for (int i = 0; i < 4; ++i)
				{
					if (coord_valida_H(x+II[i], y+JJ[i],i) and not found_in_map_S(coord_visit, x+II[i], y+JJ[i])) 			// NORD
					{
		                int dist = distancia_coord_S(coord_visit, x, y);
		                coord_visit.insert(make_pair(Position(x+II[i], y+JJ[i]), dist + 1));
		                cola.push(Position(x+II[i], y+JJ[i]));
						va[x+II[i]][y+JJ[i]]=Position(x,y);
		            }
				}
			}
			return false;
		}
	
		bool orientat_H(int id, int i, int j, int ii, int jj, int &ori_t)
		{
			if (ii == i + 1 and jj == j) ori_t = 0; 		// S
			else if (ii == i and jj == j + 1) ori_t = 1;	// E
			else if (ii == i - 1 and jj == j) ori_t = 2; 	// N
			else if (ii == i and jj == j - 1) ori_t = 3; 	// O
			
			if(ori_t==data(id).orientation) return true;
			return false;
		}

		void go_helicopters(int helicopter_id)
		{
			Data dades = data(helicopter_id);
			Position p(dades.pos.i, dades.pos.j);
			// dades de la base objectiu
			Position b;
			Position ba;

			int dist_prop=0;
			for (int pa=0; pa<P.size(); ++pa)
			{
				ba.i = P[pa].pos.i;
				ba.j = P[pa].pos.j;
				int dist = abs(p.i-ba.i) + abs(p.j-ba.j);
				if (dist>dist_prop and post_owner(ba.i,ba.j)!=me()) // major distancia
				{
					dist_prop = dist;
					b = ba;
				}
			}
			queue< Position > cola;
			int distancia;
			cola.push(p); // primera posicio
			vector<vector<Position>> vaprop(60, vector<Position>(60));
			if (BFS_H(b, cola, distancia, vaprop))
			{
				if (distancia<=3) return;
				stack<Position> recorregut;
				contruir_cami_S(recorregut, vaprop, b.i, b.j, p.i, p.j);
				int ori_t;
				if(orientat_H(helicopter_id, p.i, p.j, recorregut.top().i, recorregut.top().j, ori_t))
				{
					command_helicopter(helicopter_id, FORWARD1);
					recorregut.pop();
				}
				else 
				{
					int xd = dades.orientation - ori_t;
					if (xd == 1) command_helicopter(helicopter_id, CLOCKWISE);
					else if (xd == -1) command_helicopter(helicopter_id, COUNTER_CLOCKWISE);
					else command_helicopter(helicopter_id, CLOCKWISE);
				}
			}
		}

		void escriure_cami(stack<Position> &recorregut)
		{
			while(not recorregut.empty())
			{
				cout << recorregut.top().i << ", " << recorregut.top().j << " - ";
				recorregut.pop();
			}
			cout << endl;
		}


		void go_paracaigudes(int helicopter_id)
		{
			Data dades = data(helicopter_id);
			int ii = dades.pos.i + random(-2, 2);
			int jj = dades.pos.j + random(-2, 2);
			if(coord_valida_S(ii,jj)) command_parachuter(ii, jj);
		}

		bool fa_falta_paracaigudes(int helicopter_id)
		{
			return true;
		}

		bool fa_falta_napalm(int helicopter_id)
		{
			return false;
		}

		bool fa_falta_recalcular_sold()
		{
			if(S.size()>soldats.size()) return true;
			for (int i = 0; i < S.size(); ++i) if (soldats.find(S[i])==soldats.end()) return true;
			return false;
		}

		void recalcular_sold()
		{
			soldats.clear();
			for (int i = 0; i < S.size(); ++i)
			{
				soldat s;
				s.id = S[i];
				if (i>S.size()/4) s.rol = 0;
				else s.rol = 0;
				soldats.insert(make_pair(S[i],s));
			}
			return;
		}

		void play_helicopter(int id) 
		{
			// If we can, we throw napalm.
			// if (data(id).napalm == 0) 
			// {
			// command_helicopter(id, NAPALM);
			// return;
			// }

			// With probability 20% we turn counter clockwise,
			// otherwise we try to move forward two steps.
			int c = random(1, 5);
			command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
		}
		/**
		* Play method, invoked once per each round.
		*/
		virtual void play ()
		{
			if(status(me()) > 0.95) { return;}

			// carreguem les dades del jugador
			S = soldiers(me()); 		//  soldats del jugador
		    H = helicopters(me()); 	//	helicopters del jugador
			P = posts();

			if(fa_falta_recalcular_sold()) recalcular_sold();

			for (int i=0; i< S.size(); ++i) go_soldats(S[i]);
			//paracaigudes i helicopters
			for(int j = 0; j < data(H[0]).parachuters.size(); ++j) if(not data(H[0]).parachuters[j] or fa_falta_paracaigudes(H[0])) go_paracaigudes(H[0]);
			play_helicopter(H[0]);

			for(int j = 0; j < data(H[1]).parachuters.size(); ++j) if(not data(H[1]).parachuters[j] or fa_falta_paracaigudes(H[1])) go_paracaigudes(H[1]);
			play_helicopter(H[1]);

		}
	};


	/**
	 * Do not modify the following line.
	 */
RegisterPlayer(PLAYER_NAME);