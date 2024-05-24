#include "Player.hh"
#include <queue>
#include <map>
#include <list>
#include <iostream>

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME SedrK_v2


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

		// struct soldat
		// {
		// 	int id;
		// 	Data dades;
		// };

		typedef vector<int> VE;

		static const int DIST_MAX = 1000000;

		//map<int, soldat> soldats; // id, soldat
		vector<Post> P; // posts
		VE S; 		//  soldats del jugador
		VE H; 	//	helicopters del jugador
		VE II = {-1, 1, 0, 0,-1,-1, 1, 1 };
		VE JJ = {0, 0, -1, 1, -1, 1, -1, 1};
		queue< Position > cua_bfs; //cua pel bfs
		vector<vector<Position>> va;



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

		bool BFS_S(Position b, int &distancia)
		{
			map<Position, int, cmpare> coord_visit;
			Position aux(cua_bfs.front().i,cua_bfs.front().j);
	        coord_visit.insert(make_pair(aux, 0));
	        while (not cua_bfs.empty())
			{
	            int x = cua_bfs.front().i;
	            int y = cua_bfs.front().j;
	            cua_bfs.pop();

	            if (b.i == x and b.j == y)
				{
	                distancia = coord_visit.find(Position(x, y))->second;
	                return true;
	            }
			
	            if (coord_valida_S(x - 1, y) and coord_visit.count(Position(x - 1, y))==0) 			// NORD
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x - 1, y), dist + 1));
	                cua_bfs.push(Position(x - 1, y));
					va[x-1][y]=Position(x,y);
	            }

	            if (coord_valida_S(x + 1, y) and coord_visit.count(Position(x + 1, y))==0) 			// SUD
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x + 1, y), dist + 1));
	                cua_bfs.push(Position(x + 1, y));
					va[x+1][y]=Position(x,y);
	            }

	            if (coord_valida_S(x, y - 1 ) and coord_visit.count(Position(x, y - 1))==0) 		// OEST
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x, y - 1 ), dist + 1));
	                cua_bfs.push(Position(x, y - 1 ));
					va[x][y-1]=Position(x,y);
	            }

	            if (coord_valida_S(x, y + 1) and coord_visit.count(Position(x, y + 1))==0) 			// EST
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x, y + 1), dist + 1));
	                cua_bfs.push(Position(x, y + 1));
					va[x][y+1]=Position(x,y);
	            }

				if (coord_valida_S(x - 1, y -1) and coord_visit.count(Position(x - 1, y -1))==0) 		// NORD-OEST
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x - 1, y-1), dist + 1));
	                cua_bfs.push(Position(x - 1, y-1));
					va[x-1][y-1]=Position(x,y);
	            }

				if (coord_valida_S(x - 1, y +1) and coord_visit.count(Position(x - 1, y +1))==0)		// NORD-EST
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x - 1, y+1), dist + 1));
	                cua_bfs.push(Position(x - 1, y+1));
					va[x-1][y+1]=Position(x,y);
	            }

				if (coord_valida_S(x + 1, y -1) and coord_visit.count(Position(x + 1, y -1))==0) 	//SUD-OEST
				{
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x + 1, y-1), dist + 1));
	                cua_bfs.push(Position(x + 1, y-1));
					va[x+1][y-1]=Position(x,y);
	            }

				if (coord_valida_S(x + 1, y +1) and coord_visit.count(Position(x + 1, y +1))==0) 	//SUD-EST mymap.count(c)
				{	
	                int dist = coord_visit.find(Position(x, y))->second;
	                coord_visit.insert(make_pair(Position(x + 1, y+1), dist + 1));
	                cua_bfs.push(Position(x + 1, y+1));
					va[x+1][y+1]=Position(x,y);
	            }
	        }

	        return false;
		}

		Position contruir_cami_S(int i, int j, int ii, int jj)
		{
			while(true)
			{
				if (va[i][j].i==ii and va[i][j].j==jj) break;
				int aux = i;
				i=va[i][j].i;
				j=va[aux][j].j;
			}
			return Position(i,j);
		}

		void go_soldats(int soldat_id)
		{
			// dades del soldat
			Data s = data(soldat_id);
			Position p(data(soldat_id).pos.i, data(soldat_id).pos.j);

			// dades de la base objectiu
			Position b;
			Position ba;

			int dist_prop=DIST_MAX;
			int dist;
			
			// atacar enemics radi 1
			if(enemic_aprop(p.i,p.j,ba))
			{
				dist = abs(p.i-ba.i) + abs(p.j-ba.j);
				b = ba;
			}
			// buscar posts enemics i atacar
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
			while(not cua_bfs.empty()) cua_bfs.pop();
			cua_bfs.push(p); // primera posicio
			va = vector<vector<Position>>(60,vector<Position>(60));
			if (BFS_S(b, distancia))
			{
				if (distancia == DIST_MAX) return;
				else if (distancia == 1) command_soldier(soldat_id, b.i, b.j);
				else
				{
					Position com_pos = contruir_cami_S(b.i, b.j, p.i, p.j);
					command_soldier(soldat_id, com_pos.i, com_pos.j);
				}
			}
		}


		// bool coord_valida_H(int x, int y, int ori)
		// {
		// 	if (not pos_ok(x,y)) return false;
		// 	if (which_helicopter(x,y)>0) return false;

		// 	if (ori == 1) for (int i = -2 ; i < 2; ++i) if (not pos_ok(x+i,y+2) or what(x+i,y+2)==MOUNTAIN) return false;
		// 	else if (ori == 3) for (int i = -2; i < 2; ++i) if (not pos_ok(x+i,y-2) or what(x+i,y-2)==MOUNTAIN) return false;
		// 	else if (ori == 0) for (int i = -2; i < 2; ++i) if (not pos_ok(x+2,y+i) or what(x+2,y+i)==MOUNTAIN) return false;
		// 	else if (ori == 2) for (int i = -2; i < 2; ++i) if (not pos_ok(x-2,y+i) or what(x-2,y+i)==MOUNTAIN) return false;
		// 	else return false;
		// 	return true;
		// }

		// bool BFS_H(Position b, queue< Position > &cua_bfs, int &distancia, vector<vector<Position>> &va)
		// {
		// 	map<Position, int, cmpare> coord_visit;
		// 	Position aux(cua_bfs.front().i,cua_bfs.front().j);
	    //     coord_visit.insert(make_pair(aux, 0));
	    //     while (not cua_bfs.empty())
		// 	{
	    //         int x = cua_bfs.front().i;
	    //         int y = cua_bfs.front().j;
	    //         cua_bfs.pop();
	    //         if (b.i == x and b.j == y)
		// 		{
	    //             distancia = distancia_coord_S(coord_visit, x, y);
	    //             return true;
	    //         }
		// 		for (int i = 0; i < 4; ++i)
		// 		{
		// 			if (coord_valida_H(x+II[i], y+JJ[i],i) and not found_in_map_S(coord_visit, x+II[i], y+JJ[i])) 			// NORD
		// 			{
		//                 int dist = distancia_coord_S(coord_visit, x, y);
		//                 coord_visit.insert(make_pair(Position(x+II[i], y+JJ[i]), dist + 1));
		//                 cua_bfs.push(Position(x+II[i], y+JJ[i]));
		// 				va[x+II[i]][y+JJ[i]]=Position(x,y);
		//             }
		// 		}
		// 	}
		// 	return false;
		// }
	
		// bool orientat_H(int id, int i, int j, int ii, int jj, int &ori_t)
		// {
		// 	if (ii == i + 1 and jj == j) ori_t = 0; 		// S
		// 	else if (ii == i and jj == j + 1) ori_t = 1;	// E
		// 	else if (ii == i - 1 and jj == j) ori_t = 2; 	// N
		// 	else if (ii == i and jj == j - 1) ori_t = 3; 	// O
			
		// 	if(ori_t==data(id).orientation) return true;
		// 	return false;
		// }

		// void go_helicopters(int helicopter_id)
		// {
		// 	Data dades = data(helicopter_id);
		// 	Position p(dades.pos.i, dades.pos.j);
		// 	// dades de la base objectiu
		// 	Position b;
		// 	Position ba;

		// 	int dist_prop=0;
		// 	for (int pa=0; pa<P.size(); ++pa)
		// 	{
		// 		ba.i = P[pa].pos.i;
		// 		ba.j = P[pa].pos.j;
		// 		int dist = abs(p.i-ba.i) + abs(p.j-ba.j);
		// 		if (dist>dist_prop and post_owner(ba.i,ba.j)!=me()) // major distancia
		// 		{
		// 			dist_prop = dist;
		// 			b = ba;
		// 		}
		// 	}
		// 	queue< Position > cua_bfs;
		// 	int distancia;
		// 	cua_bfs.push(p); // primera posicio
		// 	vector<vector<Position>> va(60, vector<Position>(60));
		// 	if (BFS_H(b, cua_bfs, distancia, va))
		// 	{
		// 		if (distancia<=3) return;
		// 		stack<Position> recorregut;
		// 		contruir_cami_S(recorregut, va, b.i, b.j, p.i, p.j);
		// 		int ori_t;
		// 		if(orientat_H(helicopter_id, p.i, p.j, recorregut.top().i, recorregut.top().j, ori_t))
		// 		{
		// 			command_helicopter(helicopter_id, FORWARD1);
		// 			recorregut.pop();
		// 		}
		// 		else 
		// 		{
		// 			int xd = dades.orientation - ori_t;
		// 			if (xd == 1) command_helicopter(helicopter_id, CLOCKWISE);
		// 			else if (xd == -1) command_helicopter(helicopter_id, COUNTER_CLOCKWISE);
		// 			else command_helicopter(helicopter_id, CLOCKWISE);
		// 		}
		// 	}
		// }

		// void escriure_cami(stack<Position> &recorregut)
		// {
		// 	while(not recorregut.empty())
		// 	{
		// 		cout << recorregut.top().i << ", " << recorregut.top().j << " - ";
		// 		recorregut.pop();
		// 	}
		// 	cout << endl;
		// }


		void go_paracaigudes(int helicopter_id)
		{
			Data dades = data(helicopter_id);
			int ii = dades.pos.i + random(-2, 2);
			int jj = dades.pos.j + random(-2, 2);
			if(coord_valida_S(ii,jj)) command_parachuter(ii, jj);
		}

		// bool fa_falta_paracaigudes(int helicopter_id)
		// {
		// 	return true;
		// }

		// bool fa_falta_napalm(int helicopter_id)
		// {
		// 	return false;
		// }

		// bool fa_falta_recalcular_sold()
		// {
		// 	if(S.size()>soldats.size()) return true;
		// 	for (int i = 0; i < S.size(); ++i) if (soldats.find(S[i])==soldats.end()) return true;
		// 	return false;
		// }

		// void recalcular_sold()
		// {
		// 	soldats.clear();
		// 	for (int i = 0; i < S.size(); ++i)
		// 	{
		// 		soldat s;
		// 		s.id = S[i];
		// 		soldats.insert(make_pair(S[i],s));
		// 	}
		// 	return;
		// }

		void play_helicopter(int id) 
		{
			int c = random(1, 5);
			command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
		}

		/**
		* Play method, invoked once per each round.
		*/
	
		virtual void play ()
		{
			if(status(me()) > 0.95) return;

			// carreguem les dades del jugador
			S = soldiers(me()); 		//  soldats del jugador
		    H = helicopters(me()); 	//	helicopters del jugador
			P = posts();

			//if(fa_falta_recalcular_sold()) recalcular_sold();

			for (int i=0; i< S.size(); ++i) go_soldats(S[i]);
			//paracaigudes i helicopters
			for(int j = 0; j < data(H[0]).parachuters.size(); ++j) go_paracaigudes(H[0]);
			play_helicopter(H[0]);

			for(int j = 0; j < data(H[1]).parachuters.size(); ++j) go_paracaigudes(H[1]);
			play_helicopter(H[1]);

		}
	};


	/**
	 * Do not modify the following line.
	 */
RegisterPlayer(PLAYER_NAME);