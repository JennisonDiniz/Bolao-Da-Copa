#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
using namespace std;

// ========== FUNÇÕES ==========

int calcularPontuacao(int bolaoA, int bolaoB, int realA, int realB) {
    if (bolaoA == realA && bolaoB == realB) return 10;
    if (bolaoA == bolaoB && realA == realB) return 2;
    if ((bolaoA > bolaoB && realA > realB) ||
        (bolaoA < bolaoB && realA < realB)) return 5;
    return 0;
}

void lerPlacar(string time1, string time2, int &gol1, int &gol2) {
    while (true) {
        try {
            cout << " Gols " << time1 << ": ";
            cin >> gol1;
            cout << " Gols " << time2 << ": ";
            cin >> gol2;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                throw invalid_argument("Digite apenas numeros inteiros!");
            }
            if (gol1 < 0 || gol2 < 0) {
                throw invalid_argument("Placar nao pode ser negativo!");
            }
            break;
        } catch (invalid_argument &e) {
            cout << " ERRO: " << e.what() << " Tente novamente.\n";
        }
    }
}

void coletarPalpites(string nome, vector<int> &bolaoA, vector<int> &bolaoB,
                     string times1[], string times2[], int numJogos) {
    cout << "\n------------------------------\n";
    cout << " Palpites de " << nome << "\n";
    cout << "------------------------------\n";

    for (int i = 0; i < numJogos; i++) {
        int g1, g2;
        cout << "Jogo " << (i+1) << ": " << times1[i] << " x " << times2[i] << "\n";
        lerPlacar(times1[i], times2[i], g1, g2);
        bolaoA.push_back(g1); // insere no vetor dinâmico
        bolaoB.push_back(g2); // insere no vetor dinâmico
        cout << endl;
    }
}

int calcularTotal(vector<int> &bolaoA, vector<int> &bolaoB,
                  int realA[], int realB[], int numJogos) {
    int total = 0;
    for (int i = 0; i < numJogos; i++) {
        total += calcularPontuacao(bolaoA[i], bolaoB[i], realA[i], realB[i]);
    }
    return total;
}

void exibirResultadoJogador(string nome, vector<int> &bolaoA, vector<int> &bolaoB,
                             int realA[], int realB[],
                             string times1[], string times2[], int numJogos) {
    cout << "\n Detalhes de " << nome << ":\n";
    for (int i = 0; i < numJogos; i++) {
        int pts = calcularPontuacao(bolaoA[i], bolaoB[i], realA[i], realB[i]);
        string motivo;
        if (pts == 10) motivo = "Placar exato!";
        else if (pts == 5) motivo = "Acertou vencedor!";
        else if (pts == 2) motivo = "Acertou empate!";
        else motivo = "Errou :(";

        cout << " " << times1[i] << " x " << times2[i]
             << " | Palpite: " << bolaoA[i] << "x" << bolaoB[i]
             << " | Real: " << realA[i] << "x" << realB[i]
             << " | " << pts << "pts - " << motivo << "\n";
    }
}

// Ordena ranking por pontuação (bubble sort)
void ordenarRanking(vector<string> &nomes, vector<int> &pontos) {
    int n = pontos.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (pontos[j] < pontos[j+1]) {
                // troca pontos
                int tempPts = pontos[j];
                pontos[j] = pontos[j+1];
                pontos[j+1] = tempPts;
                // troca nomes junto
                string tempNome = nomes[j];
                nomes[j] = nomes[j+1];
                nomes[j+1] = tempNome;
            }
        }
    }
}

void exibirRanking(vector<string> &nomes, vector<int> &pontos) {
    cout << "\n==============================\n";
    cout << " RANKING FINAL \n";
    cout << "==============================\n";

    for (int i = 0; i < (int)nomes.size(); i++) {
        cout << " " << (i+1) << " lugar: "
             << nomes[i] << " - " << pontos[i] << " pts\n";
    }

    cout << "\n VENCEDOR: " << nomes[0] << "! Parabens!\n";
    cout << "==============================\n";
}

// ========== MAIN ==========
int main() {

    const int NUM_JOGOS = 4;
    string times1[NUM_JOGOS] = {"BRA", "RSA", "FRA", "GER"};
    string times2[NUM_JOGOS] = {"EGT", "MAR", "ARG", "COS"};
    int realA[NUM_JOGOS] = {3, 1, 1, 4};
    int realB[NUM_JOGOS] = {0, 1, 2, 2};

    cout << "==============================\n";
    cout << " BOLAO DA COPA 2026\n";
    cout << "==============================\n";
    cout << " 10pts = placar exato \n";
    cout << " 5pts = acertou vencedor \n";
    cout << " 2pts = acertou empate \n";
    cout << " 0pts = errou \n";
    cout << "==============================\n";

    // ===== VETORES DINÂMICOS SEPARADOS =====
    vector<string> nomes; // nomes dos jogadores
    vector<int> pontos; // pontuação total de cada jogador
    vector<vector<int>> todosA; // palpites time 1 de todos os jogadores
    vector<vector<int>> todosB; // palpites time 2 de todos os jogadores

    int numJogadores;
    cout << "\nQuantos jogadores vao participar? ";
    cin >> numJogadores;

    // ===== COLETA PALPITES DE TODOS =====
    for (int i = 0; i < numJogadores; i++) {
        string nome;
        cout << "\nNome do Jogador " << (i+1) << ": ";
        cin >> nome;
        nomes.push_back(nome); // adiciona nome no vetor

        vector<int> bolaoA, bolaoB;
        coletarPalpites(nome, bolaoA, bolaoB, times1, times2, NUM_JOGOS);

        todosA.push_back(bolaoA); // adiciona vetor de palpites no vetor de vetores
        todosB.push_back(bolaoB);
    }

    // ===== CALCULA PONTOS DE TODOS =====
    for (int i = 0; i < numJogadores; i++) {
        int total = calcularTotal(todosA[i], todosB[i], realA, realB, NUM_JOGOS);
        pontos.push_back(total); // adiciona pontuação no vetor
    }

    // ===== EXIBE RESULTADOS =====
    cout << "\n==============================\n";
    cout << " RESULTADO FINAL \n";
    cout << "==============================\n";

    for (int i = 0; i < numJogadores; i++) {
        exibirResultadoJogador(nomes[i], todosA[i], todosB[i],
                                realA, realB, times1, times2, NUM_JOGOS);
        cout << " TOTAL: " << pontos[i] << " / 40 pts\n";
    }

    // ===== ORDENA E EXIBE RANKING =====
    ordenarRanking(nomes, pontos);
    exibirRanking(nomes, pontos);

    return 0;
}