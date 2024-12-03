#include <vector>
#include <iostream>
#include <limits>

const int INF = std::numeric_limits<int>::max()/2;

class Graph{
private:
    int count_of_elem;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> l_matrix;

    void add_marking(){
        for (int i=0; i<matrix.size(); i++){
            matrix[0][i] = i;
            matrix[i][0] = i;
        }
    }

public:
    Graph(int count_of_elem): count_of_elem(count_of_elem) {
        matrix.resize(count_of_elem+1, std::vector<int>(count_of_elem+1));
        add_marking();
    }

    int get_count_of_elem(){
        return count_of_elem;
    }

    void print_graph_matrix(){
        std::cout << "\n[ADJACENCY MATRIX]\n";
        for (int i=0; i<matrix.size(); i++){
            for (int j=0; j<matrix.size(); j++){
                if(matrix[i][j] == INF) std::cout << "- ";
                else std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_l_matrix(){
        std::cout << "\n[LENGTH MATRIX]\n";
        for (int i=0; i<l_matrix.size(); i++){
            for (int j=0; j<l_matrix.size(); j++){
                if(l_matrix[i][j] == INF) std::cout << "- ";
                else std::cout << l_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void set_values(){
        int value;
        std::cout << std::endl;
        for (int i=1; i<matrix.size(); i++){
            for (int j=i+1; j<matrix.size(); j++){
                std::cout << "Enter the length between " << i << " vertex and " << j << " vertex (0 for miss): ";
                std::cin >> value;
                if (value<=0){
                    matrix[i][j] = INF;
                    matrix[j][i] = INF;
                }
                else{
                    matrix[i][j] = value;
                    matrix[j][i] = value;
                } 
            }
        }
    }

    void floyd_algorythm(){
        l_matrix = matrix;
        for (int i=1; i<l_matrix.size(); i++){
            for (int j=1; j<l_matrix.size(); j++){
                for (int k=1; k<l_matrix.size(); k++){
                    if (l_matrix[i][j] < l_matrix[i][k]+l_matrix[k][j]) l_matrix[i][j] = l_matrix[i][j];
                    else l_matrix[i][j] = l_matrix[i][k]+l_matrix[k][j];
                }
            }
        }
    }

};

int main(){
    int count_of_elem;
    std::cout << "Enter the count of vertexes in graph: ";
    std::cin >> count_of_elem;
    if (count_of_elem <= 0) return -1;
    Graph graph(count_of_elem);
    graph.print_graph_matrix();
    graph.set_values();
    graph.print_graph_matrix();
    graph.floyd_algorythm();
    graph.print_l_matrix();
    return 0;
}