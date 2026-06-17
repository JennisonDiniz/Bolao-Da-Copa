#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

const int NUM_JOGOS = 4;
const int NUM_APOSTADORES = 3;

class Jogo {                    //Representa um confronto da Copa, com os times e o placar real.
private:
    string time1, time2;
    int golsReal1, golsReal2;
    bool resultadoDefinido;

public:
    Jogo(string t1, string t2) {
        time1 = t1;
        time2 = t2;
        golsReal1 = 0;
        golsReal2 = 0;
        resultadoDefinido = false;
    }

    void definirResultado(int g1, int g2) {
        if (g1 < 0 || g2 < 0) {
            throw invalid_argument("Placar nao pode ser negativo.");
        }
        golsReal1 = g1;
        golsReal2 = g2;
        resultadoDefinido = true;
    }

    bool temResultado() const { return resultadoDefinido; }
    string getTime1() const { return time1; }
    string getTime2() const { return time2; }
    int getGolsReal1() const { return golsReal1; }
    int getGolsReal2() const { return golsReal2; }
};


//  Os palpites usam memória alocada manualmente com new[]/delete[]. 
//  Desta foram a classe controla o ciclo de vida dessa memória (construtor aloca, destrutor libera).

class Apostador {               // Guarda o nome, a pontuação total e os palpites de cada jogo.
private:
    string nome;
    int pontuacaoTotal;
    int *palpitesGol1; 
    int *palpitesGol2;  
    int numJogos;

public:
   
    Apostador(string n, int qtdJogos) {           // Construtor: aloca os vetores dinâmicos de acordo com o número de jogos
        nome = n;
        pontuacaoTotal = 0;
        numJogos = qtdJogos;
        palpitesGol1 = new int[numJogos];
        palpitesGol2 = new int[numJogos];
        for (int i = 0; i < numJogos; i++) {
            palpitesGol1[i] = -1;
            palpitesGol2[i] = -1;
        }
    }

    ~Apostador() {                                // Destrutor: libera a memória alocada no construtor
        delete[] palpitesGol1;
        delete[] palpitesGol2;
    }

    // Construtor de cópia: Isso é necessário pois a classe tem ponteiros próprios.
    // Sem isso, copiar um Apostador faria dois objetos apontarem pro mesmo bloco de memória, causando double free ou dados corrompidos.

    Apostador(const Apostador &outro) {
        nome = outro.nome;
        pontuacaoTotal = outro.pontuacaoTotal;
        numJogos = outro.numJogos;
        palpitesGol1 = new int[numJogos];
        palpitesGol2 = new int[numJogos];
        for (int i = 0; i < numJogos; i++) {
            palpitesGol1[i] = outro.palpitesGol1[i];
            palpitesGol2[i] = outro.palpitesGol2[i];
        }
    }

    // Operador de atribuição (mesma razão do construtor de cópia)
    Apostador &operator=(const Apostador &outro) {
        if (this == &outro) return *this;

        delete[] palpitesGol1;
        delete[] palpitesGol2;

        nome = outro.nome;
        pontuacaoTotal = outro.pontuacaoTotal;
        numJogos = outro.numJogos;
        palpitesGol1 = new int[numJogos];
        palpitesGol2 = new int[numJogos];
        for (int i = 0; i < numJogos; i++) {
            palpitesGol1[i] = outro.palpitesGol1[i];
            palpitesGol2[i] = outro.palpitesGol2[i];
        }
        return *this;
    }

    void definirPalpite(int indiceJogo, int g1, int g2) {
        if (indiceJogo < 0 || indiceJogo >= numJogos) {
            throw out_of_range("Indice de jogo invalido!");
        }
        if (g1 < 0 || g2 < 0) {
            throw invalid_argument("Placar nao pode ser negativo!");
        }
        palpitesGol1[indiceJogo] = g1;
        palpitesGol2[indiceJogo] = g2;
    }

    bool temPalpite(int indiceJogo) const {
        return palpitesGol1[indiceJogo] != -1;
    }

    int getPalpiteGol1(int indiceJogo) const { return palpitesGol1[indiceJogo]; }
    int getPalpiteGol2(int indiceJogo) const { return palpitesGol2[indiceJogo]; }

    string getNome() const { return nome; }
    int getPontuacaoTotal() const { return pontuacaoTotal; }
    void zerarPontuacao() { pontuacaoTotal = 0; }
    void somarPontos(int pts) { pontuacaoTotal += pts; }
};

// Regra de pontuação do bolão
int calcularPontuacao(int golApostado1, int golApostado2, int golReal1, int golReal2) {
    if (golApostado1 == golReal1 && golApostado2 == golReal2) return 10; // placar exato
    if (golApostado1 == golApostado2 && golReal1 == golReal2) return 2;  // empate
    if ((golApostado1 > golApostado2 && golReal1 > golReal2) ||
        (golApostado1 < golApostado2 && golReal1 < golReal2)) return 5; // vencedor certo
    return 0;
}

void lerDoisInteiros(const string &rotulo1, const string &rotulo2, int &v1, int &v2) {
    while (true) {
        try {
            cout << " " << rotulo1 << ": ";
            cin >> v1;
            cout << " " << rotulo2 << ": ";
            cin >> v2;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                throw invalid_argument("Digite apenas numeros inteiros!");
            }
            if (v1 < 0 || v2 < 0) {
                throw invalid_argument("Valores nao podem ser negativos!");
            }
            break;
        } catch (invalid_argument &e) {
            cout << " ERRO: " << e.what() << " Tente novamente.\n";
        }
    }
}


// Funções do Menu

void cadastrarResultados(Jogo jogos[], int qtdJogos) {
    cout << "\n------------------------------\n";
    cout << " CADASTRO DE RESULTADOS REAIS\n";
    cout << "------------------------------\n";
    for (int i = 0; i < qtdJogos; i++) {
        cout << "\nJogo " << (i + 1) << ": "
             << jogos[i].getTime1() << " x " << jogos[i].getTime2() << "\n";
        int g1, g2;
        lerDoisInteiros("Gols " + jogos[i].getTime1(),
                         "Gols " + jogos[i].getTime2(), g1, g2);
        jogos[i].definirResultado(g1, g2);
    }
    cout << "\nResultados cadastrados com sucesso!\n";
}

void cadastrarPalpites(Apostador *apostadores[], int qtdApostadores,
                        Jogo jogos[], int qtdJogos) {
    cout << "\n------------------------------\n";
    cout << " CADASTRO DE PALPITES\n";
    cout << "------------------------------\n";

    for (int a = 0; a < qtdApostadores; a++) {
        cout << "\n>> Palpites de " << apostadores[a]->getNome() << "\n";
        for (int i = 0; i < qtdJogos; i++) {
            cout << "Jogo " << (i + 1) << ": "
                 << jogos[i].getTime1() << " x " << jogos[i].getTime2() << "\n";
            int g1, g2;
            lerDoisInteiros("Palpite " + jogos[i].getTime1(),
                             "Palpite " + jogos[i].getTime2(), g1, g2);
            apostadores[a]->definirPalpite(i, g1, g2);
        }
    }
    cout << "\nPalpites cadastrados com sucesso.\n";
}

void calcularPontuacoes(Apostador *apostadores[], int qtdApostadores,
                         Jogo jogos[], int qtdJogos) {

    // A Verificação de se há resultados e palpites suficientes antes de calcular

    for (int i = 0; i < qtdJogos; i++) {
        if (!jogos[i].temResultado()) {
            cout << "\nERRO: cadastre os resultados reais antes de calcular!\n";
            return;
        }
    }

    for (int a = 0; a < qtdApostadores; a++) {
        apostadores[a]->zerarPontuacao();                       // precaução para evitar somar pontos em duplicidade se rodar 2x
        for (int i = 0; i < qtdJogos; i++) {
            if (!apostadores[a]->temPalpite(i)) {
                cout << "\nERRO: " << apostadores[a]->getNome()
                     << " ainda nao palpitou todos os jogos!\n";
                return;
            }
            int pts = calcularPontuacao(
                apostadores[a]->getPalpiteGol1(i), apostadores[a]->getPalpiteGol2(i),
                jogos[i].getGolsReal1(), jogos[i].getGolsReal2());
            apostadores[a]->somarPontos(pts);
        }
    }
    cout << "\nPontuacoes calculadas com sucesso!\n";

    cout << "\n------------------------------\n";
    cout << " DETALHES POR APOSTADOR\n";
    cout << "------------------------------\n";
    for (int a = 0; a < qtdApostadores; a++) {
        cout << "\n" << apostadores[a]->getNome() << ":\n";
        for (int i = 0; i < qtdJogos; i++) {
            int pts = calcularPontuacao(
                apostadores[a]->getPalpiteGol1(i), apostadores[a]->getPalpiteGol2(i),
                jogos[i].getGolsReal1(), jogos[i].getGolsReal2());
            cout << "  " << jogos[i].getTime1() << " x " << jogos[i].getTime2()
                 << " | Palpite: " << apostadores[a]->getPalpiteGol1(i) << "x"
                 << apostadores[a]->getPalpiteGol2(i)
                 << " | Real: " << jogos[i].getGolsReal1() << "x"
                 << jogos[i].getGolsReal2()
                 << " | " << pts << " pts\n";
        }
        cout << "  TOTAL: " << apostadores[a]->getPontuacaoTotal() << " pts\n";
    }
}

void exibirRanking(Apostador *apostadores[], int qtdApostadores) {
    
    for (int i = 0; i < qtdApostadores - 1; i++) {
        for (int j = 0; j < qtdApostadores - i - 1; j++) {
            if (apostadores[j]->getPontuacaoTotal() < apostadores[j + 1]->getPontuacaoTotal()) {
                Apostador *temp = apostadores[j];
                apostadores[j] = apostadores[j + 1];
                apostadores[j + 1] = temp;
            }
        }
    }

    cout << "\n----------------------------\n";
    cout << " RANKING FINAL\n";
    cout << "-------------------------------\n";
    for (int i = 0; i < qtdApostadores; i++) {
        cout << " " << (i + 1) << " lugar: " << apostadores[i]->getNome()
             << " - " << apostadores[i]->getPontuacaoTotal() << " pts\n";
    }
    if (qtdApostadores > 0) {
        cout << "\n VENCEDOR: " << apostadores[0]->getNome() << "Parabens!\n";
    }
}

void exibirMenu() {
    cout << "\n----------------------------\n";
    cout << " BOLAO DA COPA 2026\n";
    cout << "-------------------------------\n";
    cout << " 1 - Cadastrar resultados dos jogos\n";
    cout << " 2 - Cadastrar palpites dos apostadores\n";
    cout << " 3 - Calcular pontuacao\n";
    cout << " 4 - Exibir ranking\n";
    cout << " 0 - Sair\n";
    cout << "---------------------------------\n";
    cout << " Escolha: ";
}

int main() {
    Jogo jogos[NUM_JOGOS] = {
        Jogo("BRA", "EGT"),
        Jogo("RSA", "MAR"),
        Jogo("FRA", "ARG"),
        Jogo("GER", "COS")
    };

    Apostador *apostadores[NUM_APOSTADORES];
    for (int i = 0; i < NUM_APOSTADORES; i++) {
        apostadores[i] = nullptr;
    }

    bool apostadoresCadastrados = false;
    int opcao;

    do {
        exibirMenu();
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nOpcao invalida!\n";
            continue;
        }

        switch (opcao) {
            case 1:
                cadastrarResultados(jogos, NUM_JOGOS);
                break;

            case 2: {
                cout << "\nDigite o nome dos " << NUM_APOSTADORES << " apostadores:\n";
                for (int i = 0; i < NUM_APOSTADORES; i++) {
                    string nome;
                    cout << " Apostador " << (i + 1) << ": ";
                    cin >> nome;

                    delete apostadores[i];
                    apostadores[i] = new Apostador(nome, NUM_JOGOS);
                }
                apostadoresCadastrados = true;
                cadastrarPalpites(apostadores, NUM_APOSTADORES, jogos, NUM_JOGOS);
                break;
            }

            case 3:
                if (!apostadoresCadastrados) {
                    cout << "\nERRO: cadastre os apostadores e palpites primeiro (opcao 2)!\n";
                } else {
                    calcularPontuacoes(apostadores, NUM_APOSTADORES, jogos, NUM_JOGOS);
                }
                break;

            case 4:
                if (!apostadoresCadastrados) {
                    cout << "\nERRO: cadastre os apostadores e calcule a pontuacao primeiro!\n";
                } else {
                    exibirRanking(apostadores, NUM_APOSTADORES);
                }
                break;

            case 0:
                cout << "\nEncerrando...\n";
                break;

            default:
                cout << "\nOpcao invalida\n";
        }

    } while (opcao != 0);

    
    for (int i = 0; i < NUM_APOSTADORES; i++) {           // Libera a memória alocada para cada Apostador
        delete apostadores[i]; 
    }

    return 0;
}
