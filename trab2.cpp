#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
using namespace std;

void AplicacaoReceptora(string mensagem);
void CamadaDeAplicacaoTransmissora(string mensagem);
void AplicacaoTransmissora(void);
void MeioDeComunicacao(int fluxoBrutoDeBits []);
void CamadaEnlaceDadosTransmissoraControleDeErro (int quadro []);
void CamadaEnlaceDadosTransmissora( vector<int> &quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro []);
void CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar(int quadro []);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro []);

int main(void) {
    AplicacaoTransmissora();
    //fim do método main
}

void AplicacaoTransmissora(void) {
    //Simula o comportamento do computador A, que envia a mensagem
    string mensagem;
    cout << "Digite uma mensagem: " << endl;
    cin >> mensagem;
    //chama proxima camada
    CamadaDeAplicacaoTransmissora(mensagem); // em um exemplo mais realistico, aqui sera dado um SEND do Socket
}//fim do metodo AplicacaoTransmissora

//Função que converte string to int binario
void string_to_bin(string mensagem, vector <int> &quadro){
    string binaryString = "";
    for(int i=0;i<mensagem.size();i++)
        binaryString += (bitset<8>(mensagem[i]).to_string());
    for(int i=0;i<binaryString.size();i++)
        quadro.push_back((int)binaryString[i]);
}

void CamadaDeAplicacaoTransmissora(string mensagem) {
    vector<int> quadro;//guarda a mensagem em bits
    //Transforma a mensagem em bits
    string_to_bin(mensagem,quadro);
    //chama a próxima camada
    CamadaEnlaceDadosTransmissora(quadro);
    //AplicacaoReceptora(mensagem);
}// fim do método CamadaDeAplicacaoReceptora

void AplicacaoReceptora(string mensagem) {
    //Simula o comportamento do computador B, que recebe a mensagem
    cout << "A mensagem recebida foi:" << mensagem << endl;
} // fim do metodo AplicacaoReceptora


// Implementação meio de comunicação
/*void MeioDeComunicacao(int fluxoBrutoDeBits []) {
    //OBS: trabalhar com BITS e nao com BYTES!!!
    int erro, porcentagemDeErros;
    int fluxoBrutoDeBitsPontoA [], fluxoBrutoDeBitsPontoB [];

    porcentagemDeErros = 0; // 10% 20%, 30%, 40%, ... 100%
    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;

    while(fluxoBrutoDeBitsPontoB.lenght != fluxoBrutoDeBitsPontoA) { // fazer a probabilidade do erro
        if((rand() % 100) == "..."
            fluxoBrutoDeBitsPontoB += fluxoBrutoDeBitsPontoA; // BITS!!!
        else // ERRO! INVERTER (usa condição ternária)
            fluxoBrutoDeBitsPontoB == 0) ?
            fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB++ :
            fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB--;
    }
}*/

void encapsulamento_paridade(vector <int> &quadro,int paridade){
    int qtdd1 = 0;

    for(int i=0;i<quadro.size();i++){
        if(quadro[i])
            qtdd1++;
    }

    if(qtdd1%2 == paridade)
        quadro.push_back(0);
    else 
        quadro.push_back(1);
}

void convertBinToString(vector<int> &quadro,string &mensagem){
    mensagem = "ba";  
}

void CamadaEnlaceDadosTransmissora( vector<int> &quadro) {
    int tipoDeControle = 0;
    switch (tipoDeControle) {
        case 0: //bit de paridade par
            encapsulamento_paridade(quadro,0);
        break;
        case 1: //bit de paridade impar
            encapsulamento_paridade(quadro,1);
        break;
        case 2: //CRC
        //codigo
        //codigo
        break;
    }
    //CamadaEnlaceDadosTransmissoraControleDeErro(quadro);
    string mensagem = "";
    convertBinToString(quadro,mensagem);
    AplicacaoReceptora(mensagem);
    //chama proxima camada
} // fim do metodo CamadaEnlaceDadosTransmissora

void CamadaEnlaceDadosTransmissoraControleDeErro (int quadro []) {
    int tipoDeControleDeErro = 0; // alterar de acordo com o teste
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
        // codigo
        break;
        case 1: //bit de paridade impar
        //codigo
        break;
        case 2: //CRC
        //codigo
        //codigo
        break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro []) {
    //implementacao do algoritmo
}

void CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar(int quadro []) {
    //implementação do algoritmo
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro []) {
    //implementação do algoritmo
    //usar polinomio CRC-32 (IEEE 802)
}
