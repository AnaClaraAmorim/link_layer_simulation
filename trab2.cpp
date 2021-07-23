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
void CamadaEnlaceDadosTransmissoraControleDeErro (vector <int> &quadro);
void CamadaEnlaceDadosTransmissora( vector<int> &quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector <int> &quadro);
void CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar(vector <int> &quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro []);
void CamadaEnlaceDadosReceptora(int fluxoBrutoDeBits []);

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
    cout << "inicial:\n" << binaryString;
    for(int i=0;i<binaryString.size();i++)
        quadro.push_back(binaryString[i] - '0');
}

void CamadaDeAplicacaoTransmissora(string mensagem) {
    vector<int> quadro;//guarda a mensagem em bits
    //Transforma a mensagem em bits
    string_to_bin(mensagem,quadro);
    //chama a próxima camada
    CamadaEnlaceDadosTransmissora(quadro);
    //AplicacaoReceptora(mensagem);
}// fim do método CamadaDeAplicacaoReceptora


char ByteToChar(string str) {
    char parsed = 0;
    for (int i = 0; i < 8; i++) {
        if (str[i] == '1') {
            parsed |= 1 << (7 - i);
        }
    }
    return parsed;
}

void convertBinToString(vector<int> &quadro,string &mensagem){
    string binaryString = "";

    for (int i = 0; i < quadro.size(); i++)
        binaryString += quadro[i] + '0';

    for(int i = 0; i < binaryString.size(); i += 8) {
        mensagem += ByteToChar(binaryString.substr(i, i + 8));
    }

    cout << "\n\nmensagem convertBin: ";
    for (auto i = quadro.begin(); i != quadro.end(); ++i)
                cout << *i << " ";
    cout << "\n";
}

void desencapsulamentoParidade(vector<int> &quadro,string &mensagem){
    quadro.pop_back();
    convertBinToString(quadro,mensagem);
}

void CamadaEnlaceDadosReceptora(vector <int> &fluxoBrutoDeBits) {
    int tipoDeControle = 0;
    string mensagem = "";
    CamadaEnlaceDadosTransmissoraControleDeErro(fluxoBrutoDeBits);
    switch (tipoDeControle) {
        case 0: //bit de paridade par
            desencapsulamentoParidade(fluxoBrutoDeBits,mensagem);
        break;
        case 1: //bit de paridade impar
            desencapsulamentoParidade(fluxoBrutoDeBits,mensagem);
        break;
        case 2: //CRC
        //codigo
        //codigo
        break;
    }

    AplicacaoReceptora(mensagem);
   
   
}

void AplicacaoReceptora(string mensagem) {
    //Simula o comportamento do computador B, que recebe a mensagem
    cout << "A mensagem recebida foi:" << mensagem << endl;
} // fim do metodo AplicacaoReceptora


// Implementação meio de comunicação
void MeioDeComunicacao(vector <int> &fluxoBrutoDeBits) {
    //OBS: trabalhar com BITS e nao com BYTES!!!
    int erro;
    float porcentagemDeErros;
    int BitsSize = fluxoBrutoDeBits.size();
    int fluxoBrutoDeBitsPontoA [BitsSize];
    vector<int> fluxoBrutoDeBitsPontoB;
    int progresso = 0;

    porcentagemDeErros = 30; // 10% 20%, 30%, 40%, ... 100%
    int bitsErrados = int(porcentagemDeErros * BitsSize);
    cout << "\n\nBites:" << bitsErrados <<"\n\n";

    // Camada fisica
    //passando do tipo vector para array
    for(int i=0; i<fluxoBrutoDeBits.size(); i++) {
        fluxoBrutoDeBitsPontoA[i] = fluxoBrutoDeBits[i];  
    }

    
    while(progresso < BitsSize){
        int bit = fluxoBrutoDeBitsPontoA[progresso++];
        if(rand()%100 < porcentagemDeErros){
            cout << "entrei";
            bit = (bit + 1) % 2;
        }
        fluxoBrutoDeBitsPontoB.push_back(bit);
    }

   
    CamadaEnlaceDadosReceptora(fluxoBrutoDeBitsPontoB);
}

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

    cout << "\n\nmensagem depois: ";
    for (auto i = quadro.begin(); i != quadro.end(); ++i)
                cout << *i << " ";
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
    
    MeioDeComunicacao(quadro);
    //convertBinToString(quadro,mensagem);
    //AplicacaoReceptora(mensagem);
    //chama proxima camada
    //CamadaEnlaceDadosReceptora(quadro);
} // fim do metodo CamadaEnlaceDadosTransmissora

void CamadaEnlaceDadosTransmissoraControleDeErro (vector <int> &quadro) {
    int tipoDeControleDeErro = 0; // alterar de acordo com o teste
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
            CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
        break;
        case 1: //bit de paridade impar
            CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar(quadro);
        break;
        case 2: //CRC
        //codigo
        //codigo
        break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector <int> &quadro) {
    
    int qtdd1 = 0;

    for(int i=0;i<quadro.size()-1;i++){
        if(quadro[i])
            qtdd1++;
    }

    if(qtdd1%2 == 0 && quadro[quadro.size()-1] == 1)
        cout << "\n\nERRO DE PARIDADE PAR\n\n";
    else if(qtdd1%2 == 1 && quadro[quadro.size()-1] == 0)
        cout << "\n\nERRO DE PARIDADE PAR\n\n";

   
}

void CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar(vector <int> &quadro) {
    int qtdd1 = 0;

    for(int i=0;i<quadro.size()-1;i++){
        if(quadro[i])
            qtdd1++;
    }

    if(qtdd1%2 == 0 && quadro[quadro.size()-1] == 0)
        cout << "\n\nERRO DE PARIDADE IMPAR\n\n";
    else if(qtdd1%2 == 1 && quadro[quadro.size()-1] == 1)
        cout << "\n\nERRO DE PARIDADE IMPAR\n\n";
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro []) {
    //implementação do algoritmo
    //usar polinomio CRC-32 (IEEE 802)
}

