/* Projeto de sistema especialista
Alunos: Paulo Hermans e Pedro Garcia */

#include <iostream>
using namespace std;

enum Condicao_Oleo {
    RUIM,
    REGULAR,
    IDEAL
};

struct Estado_Freios {
    float espessura_discos; // mm
    float espessura_pastilhas; // mm
    float fluido_freio; // % (0-100)
};

// Condição do veículo num determinado momento
struct Estado {
    int km; // quilometragem
    float condicao_oleo; // % (0-100)
    float nivel_arrefecimento; // ml
    float pressao_pneu[4]; // psi
    Estado_Freios freios; // condição dos freios
};

// Condição anterior do veículo
struct Estado_Anterior {
    int km_ultima_revisao;
    float nivel_arrefecimento; // ml, intervalo de 1 mês
    float pressao_pneu[4]; // psi, intervalo de 10 min
};

// Fuzzificação e da condição do óleo
Condicao_Oleo fuzzy_oleo(float condicao_oleo) {
    // Funcão de pertinência para condição "ruim"
    float mu_ruim;
    if (condicao_oleo <= 20) {
        mu_ruim = 1;
    }
    else if (condicao_oleo < 40) {
        mu_ruim = (40 - condicao_oleo) / 20;
    }
    else {
        mu_ruim = 0;
    }

    // Funcão de pertinência para condição "regular"
    float mu_regular;
    if (condicao_oleo <= 30) {
        mu_regular = 0;
    }
    else if (condicao_oleo <= 50) {
        mu_regular = (condicao_oleo - 30) / 20;
    }
    else if (condicao_oleo < 70) {
        mu_regular = (70 - condicao_oleo) / 20;
    }
    else {
        mu_regular = 0;
    }

    // Funcão de pertinência para condição "ideal"
    float mu_ideal;
    if (condicao_oleo <= 60) {
        mu_ideal = 0;
    }
    else if (condicao_oleo < 80) {
        mu_ideal = (condicao_oleo - 60) / 20;
    }
    else {
        mu_ideal = 1;
    }

    if (mu_ruim > mu_regular && mu_ruim > mu_ideal) {
        return Condicao_Oleo::RUIM;
    }
    else if (mu_regular > mu_ruim && mu_regular > mu_ideal) {
        return Condicao_Oleo::REGULAR;
    }
    else {
        return Condicao_Oleo::IDEAL;
    }
}

// Regras do especialista, seguindo a árvore de decisão
void avaliar(const Estado& s, const Estado_Anterior& sa) {
    cout << "\nAvaliação de manutenção:\n";

    // Condição do óleo
    Condicao_Oleo cond_oleo_fuzzy = fuzzy_oleo(s.condicao_oleo);
    if (cond_oleo_fuzzy == Condicao_Oleo::RUIM) {
        cout << "Troca de óleo necessária.\n";
    }
    else if (cond_oleo_fuzzy == Condicao_Oleo::REGULAR) {
        cout << "Sugere-se a troca de óleo.\n";
    }

    // Variação do nível de arrefecimento, comparando com o limiar de 5 ml/d
    if ((sa.nivel_arrefecimento - s.nivel_arrefecimento) / 30 >= 5) {
        cout << "Verificar o radiador, reservatório e mangueiras de arrefecimento.\n";
    }

    // Pressão dos pneus
    for (int i = 0; i < 4; i++) {
        if (s.pressao_pneu[i] < 28 || s.pressao_pneu[i] > 38) {
            // Variação da pressão
            if (sa.pressao_pneu[i] - s.pressao_pneu[i] > 0.5) {
                cout << "Há um furo no pneu " << i + 1 << ". É necessária a troca.\n";
            }
            else {
                cout << "Pressão do pneu " << i + 1 << " baixa. Deve ser feita a recalibração.\n";
            }
        }
    }

    // Variável composta: condição geral dos freios
    if (s.freios.espessura_discos < 20 || s.freios.espessura_pastilhas < 4 || s.freios.fluido_freio < 75) {
        cout << "Manutenção do sistema de freios recomendada.\n";
    }

    // Quilometragem desde a última revisão geral
    if (s.km - sa.km_ultima_revisao >= 10000) {
        cout << "Revisão geral recomendada.\n";
    }

    cout << "Diagnóstico finalizado.\n";
}

// Leitura de dados (por entradas de usuário)
void ler_entrada(Estado& s) {
    cout << "Digite a quilometragem do veículo (km): ";
    cin >> s.km;

    cout << "Digite a condição do óleo (0-100): ";
    cin >> s.condicao_oleo;

    for (int i = 0; i < 4; i++) {
        cout << "Digite a pressão do pneu " << i + 1 << " (psi): ";
        cin >> s.pressao_pneu[i];
    }

    cout << "Digite o nível do líquido de arrefecimento (ml): ";
    cin >> s.nivel_arrefecimento;

    cout << "Digite a espessura dos discos de freio (mm): ";
    cin >> s.freios.espessura_discos;

    cout << "Digite a espessura das pastilhas de freio (mm): ";
    cin >> s.freios.espessura_pastilhas;

    cout << "Digite o nível do fluido de freio (0-100): ";
    cin >> s.freios.fluido_freio;
}

int main() {
    // https://github.com/alf-p-steinbach/C---how-to---make-non-English-text-work-in-Windows/blob/main/how-to-use-utf8-in-windows.md
    #ifdef _WIN32
    system("chcp 65001 >nul");
    #endif

    Estado s;

    // Definir o estado anterior do veículo com valores arbitrários
    Estado_Anterior sa;
    sa.km_ultima_revisao = 50000;
    sa.nivel_arrefecimento = 900.0;
    sa.pressao_pneu[0] = 33.0;
    sa.pressao_pneu[1] = 33.0;
    sa.pressao_pneu[2] = 33.0;
    sa.pressao_pneu[3] = 33.0;

    cout << "=== Sistema Especialista: Manutenção Veicular ===\n";
    ler_entrada(s);
    avaliar(s, sa);
    return 0;
}
