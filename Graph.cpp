#include "Graph.h"

bool _minus_one_cmp(int l, int r){
	if (r==-1 && l>-1)
		return true;
	if (l==-1 && r>-1)
		return false;
	return l<r;
}

Graph::Graph( unsigned int edgesCount, unsigned int vertexCount ){
	this->edgesCount = edgesCount;
	this->vertexCount = vertexCount;
	vertexColors = vector<int>(vertexCount,-1);
	adjacencyMatrix = new bool*[vertexCount];
	for( unsigned int i=0; i<vertexCount; i++ ){
		adjacencyMatrix[i] = new bool[vertexCount];
		for ( int j=0; j<vertexCount; j++ ){
			adjacencyMatrix[i][j] = false;
		}
	}
}

void Graph::calculateOrder(){
	for( int i=0; i<vertexCount; i++ ){
		vertexOrder.push_back(VertexOrder(i,_adjacentFor(i).size()));
	}
	sort(vertexOrder.begin(), vertexOrder.end());
}

Graph::Graph( const Graph& gToCopy ){
	edgesCount = gToCopy.edgesCount;
	vertexCount = gToCopy.vertexCount;
	vertexOrder = vector<VertexOrder>(gToCopy.vertexOrder);
	vertexColors = vector<int>(gToCopy.vertexColors);
	adjacencyMatrix = new bool*[vertexCount];
	for( unsigned int i=0; i<vertexCount; i++ ){
		adjacencyMatrix[i] = new bool[vertexCount];
		for ( int j=0; j<vertexCount; j++ ){
			adjacencyMatrix[i][j] = gToCopy.adjacencyMatrix[i][j];
		}
	}
}

void Graph::addEdge( unsigned int i, unsigned int j ){
	if ( i>=0 && i<vertexCount && j>=0 && j<vertexCount ){
		adjacencyMatrix[i][j] = true;
		adjacencyMatrix[j][i] = true;
	}
}

void Graph::removeEdge( unsigned int i, unsigned int j ){
	if ( i>=0 && i<vertexCount && j>=0 && j<vertexCount ){
		adjacencyMatrix[i][j] = false;
		adjacencyMatrix[j][i] = false;
	}
}

bool Graph::isEdge( unsigned int i, unsigned int j ){
	if ( i>=0 && i<vertexCount && j>=0 && j<vertexCount )
		return adjacencyMatrix[i][j];
	else
		return false;
}

Graph::~Graph()
{
	for ( int i=0; i<vertexCount; i++ ){
		delete[] adjacencyMatrix[i];
	}
	delete[] adjacencyMatrix;
}

vector<unsigned int> Graph::_adjacentFor(unsigned int node){
	//
	// Vector смежных узлов для <node>
	//
	vector<unsigned int> adjacent;
	for( int i=0; i<vertexCount; i++ ){
		if ( i!=node && adjacencyMatrix[node][i] )
			adjacent.push_back(i);
	}
	return adjacent;
}


int Graph::_lenPathBetween(unsigned int fromNode, unsigned int toNode, vector<Edge> edges_used, int path_len){
	//
	// Длина простого пути между вершинами.
	// Используемая версия!
	//
	int new_path_len;
	vector<unsigned int> shortest_path;
	vector<unsigned int> adj(_adjacentFor(fromNode));
	for ( int i=0; i<adj.size(); i++ ){
		if (find(edges_used.begin(),edges_used.end(),Edge(fromNode,adj[i]))==edges_used.end()){
			if (adj[i]==toNode)
				return path_len+1;
			else{
				edges_used.push_back(Edge(fromNode,adj[i]));
				edges_used.push_back(Edge(adj[i],fromNode));
				new_path_len = _lenPathBetween(adj[i],toNode, edges_used, path_len+1);
				if (new_path_len)
					shortest_path.push_back(new_path_len);
				edges_used.pop_back();
				edges_used.pop_back();
			}
		}
	}
	if (shortest_path.size()==0)
		return 0;
	else
		return *min_element(shortest_path.begin(),shortest_path.end());
}

int Graph::_lenPathBetween(unsigned int fromNode, unsigned int toNode){
	//
	// Длина простого пути между двумя вершинами.
	// -1, если цикла нет.
	// Неиспользуемая версия!
	//
	vector<unsigned int> q, verts_used;
	vector<Edge> edges_used;
	unsigned int vert, level = 0;
	q.push_back(fromNode);
	while (!q.empty()){
		vert = q[0];
		vector<unsigned int> adj(_adjacentFor(vert));
		verts_used.push_back(vert);
		q.erase(q.begin());
		level++;
		for ( size_t i=0; i<adj.size(); i++ ){
			/*if (find(edges_used.begin(),edges_used.end(),Edge(vert,adj[i]))==edges_used.end()){
				if (adj[i] == toNode)
					return level;
				else{
					q.push_back(adj[i]);
					edges_used.push_back(Edge(vert,adj[i]));
					edges_used.push_back(Edge(adj[i],vert));
				}
			}*/
			if (adj[i] == toNode)
					return level;
			if (find(verts_used.begin(),verts_used.end(),adj[i])==verts_used.end()){
				q.push_back(adj[i]);
				verts_used.push_back(adj[i]);
			}
		}
	}
	return -1;
}

unsigned int Graph::diameter(){
	//
	// Диаметр - длина максимального кратчайшего пути между двумя вершинами
	//
	vector<unsigned int> shortest_paths;
	vector<Edge> vec;
	int lenPath;
	for ( int i=0; i<vertexCount; i++ )
		for ( int j=i+1; j<vertexCount; j++ ){
			lenPath = _lenPathBetween(i,j,vec,0);
			if (lenPath>0)
				shortest_paths.push_back(lenPath);
		}
	lenPath = *max_element(shortest_paths.begin(),shortest_paths.end());
	return lenPath;
}

unsigned int Graph::girth(){
	//
	// Обхват - длина минимального простого цикла.
	// 0, если цикла нет.
	//
	vector<int> cycle_lengths;
	vector<Edge> vec;
	int cycle_len;
	for ( unsigned int i=0; i<vertexCount; i++ ){
		cycle_len = _lenPathBetween(i,i,vec,0);
		if (cycle_len)
			cycle_lengths.push_back(cycle_len);
	}
	if (cycle_lengths.size())
		return *min_element(cycle_lengths.begin(),cycle_lengths.end());
	else
		return 0;
}

bool Graph::_full_connectivity(set<unsigned int> candidates, set<unsigned int> not){
	//
	// ПРОВЕРКА: содержит ли мн-во <not> вершину,
	// соединенную со всеми вершинами <candidates>
	//
	bool full_connect;
	vector<unsigned int> adjacent;
	set<unsigned int>::iterator it1, it2;
	for ( it1=not.begin(); it1!=not.end(); it1++ ){
		full_connect = true;
		adjacent = _adjacentFor(*it1);
		for ( it2=candidates.begin(); it2!=candidates.end(); it2++ ){
			if (find(adjacent.begin(),adjacent.end(),*it2)==adjacent.end()){
				full_connect = false;
				break;
			}
		}
		if (full_connect)
			return true;
	}
	return false;
}

int Graph::_extend_compsub(set<unsigned int> compsub, set<unsigned int> candidates, set<unsigned int> not, unsigned int clique){
	//
	// Рекурсивная функция - увеличение множества вершин полного подграфа
	//
	unsigned int current_clique, new_clique;
	vector<unsigned int> adjacent;
	set<unsigned int> new_not, new_candidates;
	current_clique = clique;
	while (!candidates.empty() && !_full_connectivity(candidates, not)){
		unsigned int v = *(candidates.begin());
		compsub.insert(v);
		adjacent = _adjacentFor(v);
		set<unsigned int> adjacent_set(adjacent.begin(),adjacent.end());
		set_intersection(candidates.begin(), candidates.end(), adjacent_set.begin(), adjacent_set.end(),
			inserter(new_candidates, new_candidates.end()));
		set_intersection(not.begin(), not.end(), adjacent_set.begin(), adjacent_set.end(),
			inserter(new_not, new_not.end()));
		if (new_candidates.empty() && new_not.empty()){
			//copy(compsub.begin(), compsub.end(), ostream_iterator<unsigned int>(cout," "));
			//cout<<"\n";
			return compsub.size();
		}
		else{
			new_clique = _extend_compsub(compsub, new_candidates, new_not, current_clique);
			if (new_clique > current_clique)
				current_clique = new_clique;
		}
		compsub.erase(v);
		candidates.erase(v);
		not.insert(v);
		}
	return current_clique;
}

unsigned int Graph::clique(){
	//
	// Определение порядка максимальной клики - подграфа,
	// в котором все вершины попарно смежны (алгоритм Брона-Кербоша)
	//
	int clique_num;
	set<unsigned int> compsub, candidates, not;
	for( int i=0; i<vertexCount; i++ )
		candidates.insert(i);
	clique_num = _extend_compsub(compsub, candidates, not, 1);
	return clique_num;
}

bool Graph::_all_colored(){
	//
	// ПРОВЕРКА: окрашены ли все вершины?
	//
	for ( int i=0; i<vertexCount; i++ ){
		if (vertexColors[i]==-1)
			return false;
	}
	return true;
}

bool Graph::_adjacent_color(unsigned int vertexCode, unsigned int colorCode){
	//
	// ПРОВЕРКА: есть ли среди смежных <vertexCode> вершин окрашенные в цвет <colorCode>
	//
	vector<unsigned int> adjacent(_adjacentFor(vertexCode));
	for ( int i=0; i<adjacent.size(); i++ ){
		if (vertexColors[adjacent[i]]==colorCode)
			return true;
	}
	return false;
}

void Graph::_color_the_same(unsigned int colorCode){
	//
	// Окрасить вершины, которые возможно, в цвет <colorCode>
	//
	for ( int i=0; i<vertexCount; i++ ){
		if (vertexColors[i]==-1){
			if (!_adjacent_color(i, colorCode))
				vertexColors[i] = colorCode;
		}
	}
}

unsigned int Graph::colorNumber(){
	//
	// Хроматическое число графа - минимальное количество цветов, которое необходимо,
	// чтобы раскрасить все вершины графа так, что смежные никогда не окрашены в один цвет.
	//
	unsigned int vertexToColor, colorCode = 0;
	vector<VertexOrder> vertexOrderCp(vertexOrder);
	while (!_all_colored()){
		for ( int i=0; i<vertexOrderCp.size(); i++ ){
			if ( vertexColors[vertexOrderCp[i].num] == -1 ){
				vertexToColor = vertexOrderCp[i].num;
				break;
			}
		}
		vertexColors[vertexToColor] = colorCode;
		_color_the_same(colorCode);
		colorCode++;
	}
	return colorCode;
}

unsigned int Graph::order(){
	//
	// Порядок - число вершин.
	//
	return vertexCount;
}

unsigned int Graph::size(){
	//
	// Размер - число ребер.
	//
	return edgesCount;
}
