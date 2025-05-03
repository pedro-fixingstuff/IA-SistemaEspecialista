#include <iostream>
using namespace std;

# Estrutura de dados
struct Sensores {
    int condicao_oleo;             // 0 a 100
    int km_desde_ultima_troca;     // km
    int desgaste_pastilhas;        // 0 a 100
    int fluido_freio;              // 0 a 100
    int nivel_arrefecimento;       // 0 a 100
    bool limp_mode;                // true = 1, false = 0
};

# Fuzzificação
string fuzzificar_oleo(int valor) {
    if (valor <= 40) return "ruim";
    else if (valor <= 60) return "regular";
    else return "boa";
}

// Regras do especialista
void avaliar(const Sensores& s) {
    cout << "\nAvaliação da manutenção:\n";

    string oleo = fuzzificar_oleo(s.condicao_oleo);

    // Decisão baseada na variável fuzzy + crisp
    if (oleo == "ruim" || s.km_desde_ultima_troca > 7000) {
        cout << "Troca de óleo recomendada.\n";
    }

    // Variável composta: condição geral dos freios
    if (s.desgaste_pastilhas > 70 || s.fluido_freio < 40) {
        cout << "Manutenção do sistema de freios recomendada.\n";
    }

    // Crisp + sinal de emergência (limp mode)
    if (s.nivel_arrefecimento < 30 || s.limp_mode) {
        cout << "Verificação imediata do sistema de arrefecimento.\n";
    }

    cout << "Diagnóstico finalizado.\n";
}

// Leitura de dados (por entradas de usuário)
Sensores ler_entrada() {
    Sensores s;

    cout << "Digite a condicao do oleo (0-100): ";
    cin >> s.condicao_oleo;

    cout << "Digite a quilometragem desde a ultima troca de oleo: ";
    cin >> s.km_desde_ultima_troca;

    cout << "Digite o desgaste das pastilhas de freio (0-100): ";
    cin >> s.desgaste_pastilhas;

    cout << "Digite o nivel do fluido de freio (0-100): ";
    cin >> s.fluido_freio;

    cout << "Digite o nivel do liquido de arrefecimento (0-100): ";
    cin >> s.nivel_arrefecimento;

    int limp;
    cout << "O veiculo esta em limp mode? (1 para sim, 0 para nao): ";
    cin >> limp;
    s.limp_mode = (limp == 1);

    return s;
}

int main() {
    cout << "=== Sistema Especialista: Manutencao Veicular ===\n";
    Sensores s = ler_entrada();
    avaliar(s);
    return 0;
}
