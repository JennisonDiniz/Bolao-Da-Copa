#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
using namespace std;

// ========== CLASSE JOGO ==========

class Jogo {
private:
    string time1;
    string time2;
    int realA;
    int realB;

public:
    // Construtor
    Jogo(string t1, string t2, int ra, int rb) {
        time1 = t1;
        time2 = t2;
        realA = ra;
        realB = rb;
    }

    // Getters
    string getTime1() { return time1; }
    string getTime2() { return time2; }
    int getRealA() { return realA; }
    int getRealB() { return realB; }

    // Exibe o resultado real do jogo
    void exibirJogo() {
        cout << time1 << " x " << time2
             << " | Resultado real: " << realA << " x " << realB << "\n";
    }

    // Calcula pontuação comparando palpite com resultado real
    int calcularPontuacao(int bolaoA, int bolaoB) {
        if (bolaoA == realA && bolaoB == realB) return 10;
        if (bolaoA == bolaoB && realA == realB) return 2;
        if ((bolaoA > bolaoB && realA > realB) ||
            (bolaoA < bolaoB && realA < realB)) return 5;
        return 0;
    }
};

// ========== CLASSE APOSTADOR ==========

class Apostador {
private:
    string nome;
    vector<int> palpitesA; // gols time 1
    vector<int> palpitesB; // gols time 2
    int totalPontos;

public:
    // Construtor
    Apostador(string n) {
        nome = n;
        totalPontos = 0;
    }

    // Getter
    string getNome() { return nome; }
    int getTotalPontos() { return totalPontos; }

    // Adiciona um palpite nos vetores dinâmicos
    void adicionarPalpite(int a, int b) {
        palpitesA.push_back(a);
        palpitesB.push_back(b);
    }

    // Calcula e acumula pontos usando a classe Jogo
    void calcularPontos(vector<Jogo> &jogos) {
        totalPontos = 0;
        for (int i = 0; i < (int)jogos.size(); i++) {
            totalPontos += jogos[i].calcularPontuacao(palpitesA[i], palpitesB[i]);
        }
    }

    // Exibe detalhes de cada jogo do apostador
    void exibirResultados(vector<Jogo> &jogos) {
        cout << "\n Detalhes de " << nome << ":\n";
        for (int i = 0; i < (int)jogos.size(); i++) {
            int pts = jogos[i].calcularPontuacao(palpitesA[i], palpitesB[i]);
            string motivo;
            if (pts == 10) motivo = "Placar exato!";
            else if (pts == 5) motivo = "Acertou vencedor!";
            else if (pts == 2) motivo = "Acertou empate!";
            else motivo = "Errou :(";

            cout << " " << jogos[i].getTime1() << " x " << jogos[i].getTime2()
                 << " | Palpite: " << palpitesA[i] << "x" << palpitesB[i]
                 << " | Real: " << jogos[i].getRealA() << "x" << jogos[i].getRealB()
                 << " | " << pts << "pts - " << motivo << "\n";
        }
        cout << " TOTAL: " << totalPontos << " / 40 pts\n";
    }
};

// ========== FUNÇÕES AUXILIARES ==========

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

// Ordena apostadores por pontuação (bubble sort)
void ordenarRanking(vector<Apostador> &apostadores) {
    int n = apostadores.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (apostadores[j].getTotalPontos() < apostadores[j+1].getTotalPontos()) {
                Apostador temp = apostadores[j];
                apostadores[j] = apostadores[j+1];
                apostadores[j+1] = temp;
            }
        }
    }
}

void exibirRanking(vector<Apostador> &apostadores) {
    cout << "\n==============================\n";
    cout << " RANKING FINAL \n";
    cout << "==============================\n";

    for (int i = 0; i < (int)apostadores.size(); i++) {
        cout << " " << (i+1) << " lugar: "
             << apostadores[i].getNome()
             << " - " << apostadores[i].getTotalPontos() << " pts\n";
    }

    cout << "\n VENCEDOR: " << apostadores[0].getNome() << "! Parabens!\n";
    cout << "==============================\n";
}

// ========== MAIN ==========
int main() {

    // ===== CRIA OS JOGOS COM A CLASSE JOGO =====
    vector<Jogo> jogos;
    jogos.push_back(Jogo("BRA", "EGT", 3, 0));
    jogos.push_back(Jogo("RSA", "MAR", 1, 1));
    jogos.push_back(Jogo("FRA", "ARG", 1, 2));
    jogos.push_back(Jogo("GER", "COS", 4, 2));

    cout << "==============================\n";
    cout << " BOLAO DA COPA 2026\n";
    cout << "==============================\n";
    cout << " 10pts = placar exato \n";
    cout << " 5pts = acertou vencedor \n";
    cout << " 2pts = acertou empate \n";
    cout << " 0pts = errou \n";
    cout << "==============================\n";

    cout << "\nJogos da rodada:\n";
    for (int i = 0; i < (int)jogos.size(); i++) {
        cout << " Jogo " << (i+1) << ": ";
        jogos[i].exibirJogo();
    }

    // ===== CRIA OS APOSTADORES COM A CLASSE APOSTADOR =====
    vector<Apostador> apostadores;

    int numApostadores;
    cout << "\nQuantos apostadores vao participar? ";
    cin >> numApostadores;

    for (int i = 0; i < numApostadores; i++) {
        string nome;
        cout << "\nNome do Apostador " << (i+1) << ": ";
        cin >> nome;

        Apostador apostador(nome); // cria objeto da classe Apostador

        cout << "\n------------------------------\n";
        cout << " Palpites de " << nome << "\n";
        cout << "------------------------------\n";

        for (int j = 0; j < (int)jogos.size(); j++) {
            int g1, g2;
            cout << "Jogo " << (j+1) << ": "
                 << jogos[j].getTime1() << " x " << jogos[j].getTime2() << "\n";
            lerPlacar(jogos[j].getTime1(), jogos[j].getTime2(), g1, g2);
            apostador.adicionarPalpite(g1, g2); // método da classe Apostador
            cout << endl;
        }

        apostador.calcularPontos(jogos); // método da classe Apostador
        apostadores.push_back(apostador); // adiciona no vetor dinâmico
    }

    // ===== EXIBE RESULTADOS =====
    cout << "\n==============================\n";
    cout << " RESULTADO FINAL \n";
    cout << "==============================\n";

    for (int i = 0; i < (int)apostadores.size(); i++) {
        apostadores[i].exibirResultados(jogos); // método da classe Apostador
    }

    // ===== ORDENA E EXIBE RANKING =====
    ordenarRanking(apostadores);
    exibirRanking(apostadores);

    return 0;
}