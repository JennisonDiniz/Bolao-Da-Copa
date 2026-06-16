#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// ========== CLASSE JOGO ==========

class Jogo {
private:
    string time1;
    string time2;
    int realA;
    int realB;
    bool cadastrado;

public:
    Jogo() {
        time1 = "";
        time2 = "";
        realA = 0;
        realB = 0;
        cadastrado = false;
    }

    Jogo(string t1, string t2, int ra, int rb) {
        time1 = t1;
        time2 = t2;
        realA = ra;
        realB = rb;
        cadastrado = true;
    }

    // Getters
    string getTime1() { return time1; }
    string getTime2() { return time2; }
    int getRealA() { return realA; }
    int getRealB() { return realB; }
    bool isCadastrado() { return cadastrado; }

    // Setters para cadastrar via menu
    void setJogo(string t1, string t2, int ra, int rb) {
        time1 = t1;
        time2 = t2;
        realA = ra;
        realB = rb;
        cadastrado = true;
    }

    void exibirJogo(int num) {
        cout << " Jogo " << num << ": " << time1 << " x " << time2;
        if (cadastrado)
            cout << " | Resultado: " << realA << " x " << realB;
        else
            cout << " | Resultado: (nao cadastrado)";
        cout << "\n";
    }

    int calcularPontuacao(int bolaoA, int bolaoB) {
        if (!cadastrado) return 0;
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
    int *palpitesA; // vetor dinâmico com new[]
    int *palpitesB; // vetor dinâmico com new[]
    int totalPontos;
    int numJogos;
    bool palpitesCadastrados;

public:
    // Construtor — aloca memória dinâmica
    Apostador(string n, int nj) {
        nome = n;
        numJogos = nj;
        totalPontos = 0;
        palpitesCadastrados = false;
        palpitesA = new int[numJogos]; // alocação dinâmica com new[]
        palpitesB = new int[numJogos]; // alocação dinâmica com new[]
        for (int i = 0; i < numJogos; i++) {
            palpitesA[i] = 0;
            palpitesB[i] = 0;
        }
    }

    // Destrutor — libera memória alocada com delete[]
    ~Apostador() {
        delete[] palpitesA;
        delete[] palpitesB;
        cout << " [Memoria de " << nome << " liberada com delete[]]\n";
    }

    // Getters
    string getNome() { return nome; }
    int getTotalPontos() { return totalPontos; }
    bool isPalpiteCadastrado() { return palpitesCadastrados; }

    void adicionarPalpites(int *a, int *b) {
        for (int i = 0; i < numJogos; i++) {
            palpitesA[i] = a[i];
            palpitesB[i] = b[i];
        }
        palpitesCadastrados = true;
    }

    void calcularPontos(Jogo jogos[], int n) {
        totalPontos = 0;
        for (int i = 0; i < n; i++) {
            totalPontos += jogos[i].calcularPontuacao(palpitesA[i], palpitesB[i]);
        }
    }

    void exibirResultados(Jogo jogos[], int n) {
        cout << "\n Apostador: " << nome << "\n";
        if (!palpitesCadastrados) {
            cout << " (sem palpites cadastrados)\n";
            return;
        }
        for (int i = 0; i < n; i++) {
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
            if (gol1 < 0 || gol2 < 0)
                throw invalid_argument("Placar nao pode ser negativo!");
            break;
        } catch (invalid_argument &e) {
            cout << " ERRO: " << e.what() << " Tente novamente.\n";
        }
    }
}

void ordenarRanking(Apostador **apostadores, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (apostadores[j]->getTotalPontos() < apostadores[j+1]->getTotalPontos()) {
                Apostador *temp = apostadores[j];
                apostadores[j] = apostadores[j+1];
                apostadores[j+1] = temp;
            }
        }
    }
}

// ========== MENUS ==========

void menuCadastrarJogos(Jogo jogos[], int numJogos) {
    cout << "\n--- Cadastrar Resultados dos Jogos ---\n";
    for (int i = 0; i < numJogos; i++) {
        string t1, t2;
        int ra, rb;
        cout << "\nJogo " << (i+1) << ":\n";
        cout << " Time 1: "; cin >> t1;
        cout << " Time 2: "; cin >> t2;
        lerPlacar(t1, t2, ra, rb);
        jogos[i].setJogo(t1, t2, ra, rb);
        cout << " Jogo cadastrado!\n";
    }
}

void menuCadastrarPalpites(Jogo jogos[], int numJogos,
                            Apostador **apostadores, int numApostadores) {
    if (!jogos[0].isCadastrado()) {
        cout << "\n AVISO: Cadastre os jogos primeiro!\n";
        return;
    }
    cout << "\n--- Cadastrar Palpites dos Apostadores ---\n";
    for (int i = 0; i < numApostadores; i++) {
        cout << "\nPalpites de " << apostadores[i]->getNome() << ":\n";
        int *a = new int[numJogos];
        int *b = new int[numJogos];
        for (int j = 0; j < numJogos; j++) {
            cout << "Jogo " << (j+1) << ": "
                 << jogos[j].getTime1() << " x " << jogos[j].getTime2() << "\n";
            lerPlacar(jogos[j].getTime1(), jogos[j].getTime2(), a[j], b[j]);
        }
        apostadores[i]->adicionarPalpites(a, b);
        delete[] a; // libera memória temporária
        delete[] b;
        cout << " Palpites de " << apostadores[i]->getNome() << " cadastrados!\n";
    }
}

void menuCalcularPontuacao(Jogo jogos[], int numJogos,
                            Apostador **apostadores, int numApostadores) {
    if (!apostadores[0]->isPalpiteCadastrado()) {
        cout << "\n AVISO: Cadastre os palpites primeiro!\n";
        return;
    }
    cout << "\n--- Calculando Pontuacao ---\n";
    for (int i = 0; i < numApostadores; i++) {
        apostadores[i]->calcularPontos(jogos, numJogos);
        cout << " " << apostadores[i]->getNome()
             << ": " << apostadores[i]->getTotalPontos() << " pts\n";
    }
}

void menuExibirRanking(Jogo jogos[], int numJogos,
                        Apostador **apostadores, int numApostadores) {
    if (!apostadores[0]->isPalpiteCadastrado()) {
        cout << "\n AVISO: Cadastre os palpites e calcule a pontuacao primeiro!\n";
        return;
    }

    cout << "\n--- Resultado por Apostador ---\n";
    for (int i = 0; i < numApostadores; i++) {
        apostadores[i]->exibirResultados(jogos, numJogos);
    }

    ordenarRanking(apostadores, numApostadores);

    cout << "\n==============================\n";
    cout << " RANKING FINAL \n";
    cout << "==============================\n";
    for (int i = 0; i < numApostadores; i++) {
        cout << " " << (i+1) << " lugar: "
             << apostadores[i]->getNome()
             << " - " << apostadores[i]->getTotalPontos() << " pts\n";
    }
    cout << "\n VENCEDOR: " << apostadores[0]->getNome() << "! Parabens!\n";
    cout << "==============================\n";
}

// ========== MAIN ==========
int main() {

    const int NUM_JOGOS = 4; // vetor estático de jogos
    Jogo jogos[NUM_JOGOS]; // vetor estático com new não necessário

    cout << "==============================\n";
    cout << " BOLAO DA COPA 2026\n";
    cout << "==============================\n";
    cout << " 10pts = placar exato \n";
    cout << " 5pts = acertou vencedor \n";
    cout << " 2pts = acertou empate \n";
    cout << " 0pts = errou \n";
    cout << "==============================\n";

    // ===== CADASTRO DE APOSTADORES =====
    int numApostadores;
    cout << "\nQuantos apostadores vao participar? ";
    cin >> numApostadores;

    // Vetor dinâmico de ponteiros para Apostador com new[]
    Apostador **apostadores = new Apostador*[numApostadores];
    for (int i = 0; i < numApostadores; i++) {
        string nome;
        cout << "Nome do Apostador " << (i+1) << ": ";
        cin >> nome;
        apostadores[i] = new Apostador(nome, NUM_JOGOS); // new para cada objeto
    }

    // ===== MENU PRINCIPAL =====
    int opcao = 0;
    do {
        cout << "\n==============================\n";
        cout << " MENU PRINCIPAL \n";
        cout << "==============================\n";
        cout << " 1. Cadastrar resultados dos jogos\n";
        cout << " 2. Cadastrar palpites dos apostadores\n";
        cout << " 3. Calcular pontuacao\n";
        cout << " 4. Exibir ranking\n";
        cout << " 0. Sair\n";
        cout << "------------------------------\n";
        cout << " Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                menuCadastrarJogos(jogos, NUM_JOGOS);
                break;
            case 2:
                menuCadastrarPalpites(jogos, NUM_JOGOS, apostadores, numApostadores);
                break;
            case 3:
                menuCalcularPontuacao(jogos, NUM_JOGOS, apostadores, numApostadores);
                break;
            case 4:
                menuExibirRanking(jogos, NUM_JOGOS, apostadores, numApostadores);
                break;
            case 0:
                cout << "\nEncerrando programa...\n\n";
                break;
            default:
                cout << "\n Opcao invalida! Tente novamente.\n";
        }
    } while (opcao != 0);

    // ===== LIBERA MEMÓRIA COM delete[] =====
    cout << "\n--- Liberando memoria ---\n";
    for (int i = 0; i < numApostadores; i++) {
        delete apostadores[i]; // chama o destrutor da classe Apostador
    }
    delete[] apostadores; // libera o vetor de ponteiros
    cout << "Memoria liberada com sucesso!\n";

    return 0;
}