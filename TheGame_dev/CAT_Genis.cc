#include "Player.hh"
#include <queue>
#include <map>
#include <list>
#include <iostream>

/**
* Write the name of your player and save this file
* with the same name and .cc extension.
*/
#define PLAYER_NAME CAT_Genis


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

		typedef vector<int> VE;

		static const int DIST_MAX = 1000000;
		const VE II = {1, 0, -1, 0,-1,-1, 1, 1 };
		const VE JJ = {0, 1, 0, -1, -1, 1, -1, 1};
		vector<vector<Position>> va;

		bool enemic_aprop(int x, int y, Position &en)
		{
			for(int i=x-1; i<=x+1; ++i)
			{
				for(int j=y-1; j<=y+1; ++j)
				{
					if(pos_ok(i,j))
					{
						int s = which_soldier(i,j);
						if (s>0 and data(s).player!=me()) 
						{
							en.i=i; en.j=j;
							return true;
						}
					}
				}
			}
			return false;
		}

		bool post_adjacent(int x, int y, Position &en)
		{
			for(int i=x-1; i<=x+1; ++i)
			{
				for(int j=y-1; j<=y+1; ++j)
				{
					if(pos_ok(i,j))
					{
						int s = post_owner(i,j);
						if (s > -1 and s!=me()) 
						{
							en.i=i; en.j=j;
							return true;
						}
					}
				}
			}
			return false;
		}

		bool enemic_aliat_aprop(int x, int y)
		{
			int ca = 0;
			int ce = 0;
			for(int i=x-1; i<=x+1; ++i)
			{
				for(int j=y-1; j<=y+1; ++j)
				{
					int s = which_soldier(i,j);
					if (s>0 and data(s).player==me()) ++ca;
					if (s>0 and data(s).player!=me()) ++ce;
				}
			}
			return ce-ca >= 2;
		}

		bool coord_valida_S(int x, int y)
		{
			return pos_ok(x,y) and not(what(x,y)==MOUNTAIN or what(x,y)==WATER or fire_time(x,y)>0);
		}

		bool BFS_S(Position b, queue< Position > &cua_bfs)
		{
			va[cua_bfs.front().i][cua_bfs.front().j] = Position(cua_bfs.front().i,cua_bfs.front().j);

	        while (not cua_bfs.empty())
			{
	            int x = cua_bfs.front().i;
	            int y = cua_bfs.front().j;
	            cua_bfs.pop();

	            if (b.i == x and b.j == y) return true;
			
				for (int i=0; i<8; ++i)
				{
					if (coord_valida_S(x+II[i], y+JJ[i]) and (va[x+II[i]][y+JJ[i]].i==-1 and va[x+II[i]][y+JJ[i]].j==-1)) 
					{
						cua_bfs.push(Position(x+II[i], y+JJ[i]));
						va[x+II[i]][y+JJ[i]]=Position(x,y);
					}
				}
	        }

	        return false;
		}

		Position contruir_cami_S(int i, int j, int ii, int jj)
		{
			while(true)
			{
				if (va[i][j].i==ii and va[i][j].j==jj) return Position(i,j);;
				int aux = i;
				i=va[i][j].i;
				j=va[aux][j].j;
			}
			return Position(-1,-1);
		}

		void go_soldats(int soldat_id)
		{
			// dades del soldat
			Position p(data(soldat_id).pos.i, data(soldat_id).pos.j);

			// dades de la base objectiu
			Position b;
			Position ba;

			int dist_prop=DIST_MAX;
			int dist;
			
			// atacar enemics radi 1
			if(enemic_aprop(p.i,p.j,ba))
			{
				command_soldier(soldat_id, ba.i, ba.j);
				return;
			}
			if(post_adjacent(p.i,p.j,ba))
			{
				command_soldier(soldat_id, ba.i, ba.j);
				return;
			}
			// buscar posts enemics i atacar
			for (int pa=0; pa<posts().size(); ++pa)
			{
				ba.i = posts()[pa].pos.i;
				ba.j = posts()[pa].pos.j;
				dist = abs(p.i-ba.i) + abs(p.j-ba.j);
				if (dist<dist_prop and post_owner(ba.i, ba.j)!= me() ) // menor distancia i no hi ha ningu
				{
					dist_prop = dist;
					b = ba;
				}
			}
			queue< Position > cua_bfs; //cua pel bfs
			cua_bfs.push(p); // primera posicio
			va = vector<vector<Position>>(60,vector<Position>(60,Position(-1,-1)));
			if (BFS_S(b, cua_bfs))
			{
				Position com_pos = contruir_cami_S(b.i, b.j, p.i, p.j);
				command_soldier(soldat_id, com_pos.i, com_pos.j);
				return;
			}
		}


		bool coord_valida_H(int x, int y, int ori)
		{
			if (not pos_ok(x,y)) return false;
			if (which_helicopter(x,y)>0) return false;

			else if (ori == 1)  // E
			{
				for (int i = -5 ; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if(which_helicopter(x+i,y+4)>0) return false;
					else if(which_helicopter(x+i,y+5)>0) return false;
				}
			}
			else if (ori == 3) // O
			{
				for (int i = -5; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if (which_helicopter(x+i,y-4)>0) return false;
					else if(which_helicopter(x+i,y-5)>0) return false;
				}
			}
			else if (ori == 0)  //S
			{
				for (int i = -5; i <= 5; ++i) 
				{	
					if (i==5 or i==-5) if(which_helicopter(x+4,y+i)>0) return false;
					else if(which_helicopter(x+5,y+i)>0) return false;
				}
			}
			else if (ori == 2) // N
			{
				for (int i = -5; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if(which_helicopter(x-4,y+i)>0) return false;
					else if(which_helicopter(x-5,y+i)>0) return false;
				}
			}
			else return false;
			return true;
		}

		bool coord_valida_2(int x, int y, int ori)
		{
			if (not pos_ok(x,y)) return false;
			if (which_helicopter(x,y)>0) return false;

			else if (ori == 1)  // E
			{
				for (int i = -5 ; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if(which_helicopter(x+i,y+5)>0) return false;
					else if(which_helicopter(x+i,y+5)>0) return false;
				}
			}
			else if (ori == 3) // O
			{
				for (int i = -5; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if (which_helicopter(x+i,y-5)>0) return false;
					else if(which_helicopter(x+i,y-5)>0) return false;
				}
			}
			else if (ori == 0)  //S
			{
				for (int i = -5; i <= 5; ++i) 
				{	
					if (i==5 or i==-5) if(which_helicopter(x+4,y+i)>0) return false;
					else if(which_helicopter(x+5,y+i)>0) return false;
				}
			}
			else if (ori == 2) // N
			{
				for (int i = -5; i <= 5; ++i) 
				{
					if (i==5 or i==-5) if(which_helicopter(x-5,y+i)>0) return false;
					else if(which_helicopter(x-5,y+i)>0) return false;
				}
			}
			else return false;
			return true;
		}

		bool coord_valida_H_2(int x, int y, int ori)
		{
			if (not pos_ok(x,y)) return false;
			if (which_helicopter(x,y)>0) return false;

			else if (ori == 1)  // E
			{
				for (int i = -2 ; i <= 2; ++i) 
				{
					if (not pos_ok(x+i,y+2) or what(x+i,y+2)==MOUNTAIN) return false;
				}
			}
			else if (ori == 3) // O
			{
				for (int i = -2; i <= 2; ++i) 
				{
					if (not pos_ok(x+i,y-2) or what(x+i,y-2)==MOUNTAIN) return false;
				}
			}
			else if (ori == 0)  //S
			{
				for (int i = -2; i <= 2; ++i) 
				{
					if (not pos_ok(x+2,y+i) or what(x+2,y+i)==MOUNTAIN) return false;
				}
			}
			else if (ori == 2) // N
			{
				for (int i = -2; i <= 2; ++i) 
				{
					if (not pos_ok(x-2,y+i) or what(x-2,y+i)==MOUNTAIN) return false;
				}
			}
			else return false;
			return true;
		}

		bool BFS_H(Position b, queue< Position > &cua_bfs)
		{
			va[cua_bfs.front().i][cua_bfs.front().j] = Position(cua_bfs.front().i,cua_bfs.front().j);

	        while (not cua_bfs.empty())
			{
	            int x = cua_bfs.front().i;
	            int y = cua_bfs.front().j;
	            cua_bfs.pop();

	            if (b.i == x and b.j == y) return true;
			
				for (int i=0; i<4; ++i)
				{
					if (coord_valida_H_2(x+II[i], y+JJ[i], i) and (va[x+II[i]][y+JJ[i]].i==-1 and va[x+II[i]][y+JJ[i]].j==-1)) 
					{
						cua_bfs.push(Position(x+II[i], y+JJ[i]));
						va[x+II[i]][y+JJ[i]]=Position(x,y);
					}
				}
	        }

	        return false;
		}
	
		bool orientat_H(int id, int i, int j, int ii, int jj, int &orit)
		{
			if (ii == i + 1 and jj == j) orit = 0; 		// S
			else if (ii == i and jj == j + 1) orit = 1;	// E
			else if (ii == i - 1 and jj == j) orit = 2; 	// N
			else if (ii == i and jj == j - 1) orit = 3; 	// O
			
			if(orit==data(id).orientation) return true;
			return false;
		}

		void go_helicopters(int helicopter_id)
		{

			if (enemic_aliat_aprop(data(helicopter_id).pos.i, data(helicopter_id).pos.j) and data(helicopter_id).napalm == 0) 
			{
				command_helicopter(helicopter_id, NAPALM); 
				return;
			}

			Position p(data(helicopter_id).pos.i, data(helicopter_id).pos.j);

			// dades de la base objectiu
			Position b;
			Position ba;

			int dist_prop=DIST_MAX;
			for (int pa=0; pa<posts().size(); ++pa)
			{
				ba.i = posts()[pa].pos.i;
				ba.j = posts()[pa].pos.j;
				int dist = abs(p.i-ba.i) + abs(p.j-ba.j);
				if (dist<dist_prop and post_owner(ba.i,ba.j)!=me()) // menor distancia
				{
					dist_prop = dist;
					b = ba;
				}
			}
			bool trobat = false;
			for(int i = p.i-2; i < p.i+2; ++i)
			{
				for(int j = p.j-2; j < p.j+2; ++j)
				{
					if (i==b.i and j==b.j)return;
				}
			}
		
			queue< Position > cua_bfs; //cua pel bfs
			cua_bfs.push(p); // primera posicio
			va = vector<vector<Position>>(60,vector<Position>(60,Position(-1,-1)));
			if (BFS_H(b, cua_bfs))
			{
				Position com_pos = contruir_cami_S(b.i, b.j, p.i, p.j);
				int orit;
				bool orien = orientat_H(helicopter_id, p.i, p.j, com_pos.i, com_pos.j, orit);

				if(orien)
				{
					if (coord_valida_H(com_pos.i, com_pos.j,orit))
					{
						if(coord_valida_2(com_pos.i, com_pos.j,orit)) command_helicopter(helicopter_id, FORWARD2);
						else command_helicopter(helicopter_id, FORWARD1);
						return;
					}
						
				}
				else 
				{
					int xd = data(helicopter_id).orientation - orit;
					if (xd == 1 or xd == 2 or xd == -3) command_helicopter(helicopter_id, CLOCKWISE);
					else if (xd == -1 or xd == -2 or xd == 3) command_helicopter(helicopter_id, COUNTER_CLOCKWISE);
					return;
				}
			}
		
			
		}

		void go_paracaigudes(int helicopter_id)
		{
			Data dades = data(helicopter_id);
			int ii = dades.pos.i + random(-2, 2);
			int jj = dades.pos.j + random(-2, 2);
			if(coord_valida_S(ii,jj)) command_parachuter(ii, jj);
			return;
		}

		void play_helicopters(int id) 
		{
			if (enemic_aliat_aprop(data(id).pos.i, data(id).pos.j) and data(id).napalm == 0) {command_helicopter(id, NAPALM); return;}
			else
			{
				int c = random(1, 2);
				command_helicopter(id, c == 1 ? COUNTER_CLOCKWISE : FORWARD2);
				return;
			}
		}

		/**
		* Play method, invoked once per each round.
		*/
	
		virtual void play ()
		{
			if(status(me()) > 0.96) return;

			for (int i=0; i< soldiers(me()).size(); ++i) go_soldats(soldiers(me())[i]);

			//paracaigudes i helicopters
			for(int j = 0; j < data(helicopters(me())[0]).parachuters.size(); ++j) go_paracaigudes(helicopters(me())[0]);
			go_helicopters(helicopters(me())[0]);
			//play_helicopters(helicopters(me())[0]);


			for(int j = 0; j < data(helicopters(me())[1]).parachuters.size(); ++j) go_paracaigudes(helicopters(me())[1]);
			go_helicopters(helicopters(me())[1]);
			//play_helicopters(helicopters(me())[1]);
		}
	};


	/**
	 * Do not modify the following line.
	 */
RegisterPlayer(PLAYER_NAME);