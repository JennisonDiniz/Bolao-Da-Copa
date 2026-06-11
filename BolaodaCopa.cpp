#include <iostream>
#include <string>
#include <stdexcept>

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

void coletarPalpites(string nome, int bolaoA[], int bolaoB[],
                     string times1[], string times2[], int numJogos) {
    cout << "\n------------------------------\n";
    cout << " Palpites de " << nome << "\n";
    cout << "------------------------------\n";
    for (int i = 0; i < numJogos; i++) {
        cout << "Jogo " << (i+1) << ": " << times1[i] << " x " << times2[i] << "\n";
        lerPlacar(times1[i], times2[i], bolaoA[i], bolaoB[i]);
        cout << endl;
    }
}

int calcularTotal(int bolaoA[], int bolaoB[],
                  int realA[], int realB[], int numJogos) {
    int total = 0;
    for (int i = 0; i < numJogos; i++) {
        total += calcularPontuacao(bolaoA[i], bolaoB[i], realA[i], realB[i]);
    }
    return total;
}

void exibirResultadoJogador(string nome, int bolaoA[], int bolaoB[],
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

void exibirRanking(string nome1, int pts1, string nome2, int pts2) {
    cout << "\n==============================\n";
    cout << " RANKING FINAL \n";
    cout << "==============================\n";

    if (pts1 > pts2) {
        cout << " 1 lugar: " << nome1 << " - " << pts1 << " pts\n";
        cout << " 2 lugar: " << nome2 << " - " << pts2 << " pts\n";
        cout << "\n VENCEDOR: " << nome1 << "! Parabens!\n";
    } else if (pts2 > pts1) {
        cout << " 1 lugar: " << nome2 << " - " << pts2 << " pts\n";
        cout << " 2 lugar: " << nome1 << " - " << pts1 << " pts\n";
        cout << "\n VENCEDOR: " << nome2 << "! Parabens!\n";
    } else {
        cout << " " << nome1 << ": " << pts1 << " pts\n";
        cout << " " << nome2 << ": " << pts2 << " pts\n";
        cout << "\n EMPATE! Os dois acertaram igual!\n";
    }
    cout << "==============================\n";
}

// ========== MAIN ==========
int main() {

    const int NUM_JOGOS = 4;
    string times1[NUM_JOGOS] = {"BRA", "RSA", "FRA", "GER"};
    string times2[NUM_JOGOS] = {"EGT", "MAR", "ARG", "COS"};
    int realA[NUM_JOGOS] = {3, 1, 1, 4};
    int realB[NUM_JOGOS] = {0, 1, 2, 2};

    int bolaoA1[NUM_JOGOS], bolaoB1[NUM_JOGOS]; // palpites jogador 1
    int bolaoA2[NUM_JOGOS], bolaoB2[NUM_JOGOS]; // palpites jogador 2
    string nome1, nome2;

    cout << "==============================\n";
    cout << " BOLAO DA COPA 2026 \n";
    cout << "==============================\n";
    cout << " 10pts = placar exato \n";
    cout << " 5pts = acertou vencedor \n";
    cout << " 2pts = acertou empate \n";
    cout << " 0pts = errou \n";
    cout << "==============================\n";

    // ===== JOGADOR 1 =====
    cout << "\nNome do Jogador 1: ";
    cin >> nome1;
    coletarPalpites(nome1, bolaoA1, bolaoB1, times1, times2, NUM_JOGOS);

    // ===== JOGADOR 2 =====
    cout << "\nNome do Jogador 2: ";
    cin >> nome2;
    coletarPalpites(nome2, bolaoA2, bolaoB2, times1, times2, NUM_JOGOS);

    // ===== RESULTADOS =====
    int pts1 = calcularTotal(bolaoA1, bolaoB1, realA, realB, NUM_JOGOS);
    int pts2 = calcularTotal(bolaoA2, bolaoB2, realA, realB, NUM_JOGOS);

    cout << "\n==============================\n";
    cout << " RESULTADO FINAL \n";
    cout << "==============================\n";

    exibirResultadoJogador(nome1, bolaoA1, bolaoB1, realA, realB, times1, times2, NUM_JOGOS);
    cout << " TOTAL " << nome1 << ": " << pts1 << " / 40 pts\n";

    exibirResultadoJogador(nome2, bolaoA2, bolaoB2, realA, realB, times1, times2, NUM_JOGOS);
    cout << " TOTAL " << nome2 << ": " << pts2 << " / 40 pts\n";

    // ===== RANKING =====
    exibirRanking(nome1, pts1, nome2, pts2);

    return 0;
}