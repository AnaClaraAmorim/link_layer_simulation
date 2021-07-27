/*Programa feito para simular a camada de enlace
Ou seja, simular o envio de uma mensagem de um computador A para um computador B*/

#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>

using namespace std;
#define POLYNOMESIZE 32

void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<int> &quadro);
void CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> &quadro);
void MeioDeComunicacao(vector<int> &fluxoBrutoDeBits);
void CamadaEnlaceDadosReceptora(vector<int> &quadro);
void CamadaAplicacaoReceptora(vector<int> &quadro);
void AplicacaoReceptora(string mensagem);
void string_to_bin(string mensagem, vector<int> &quadro);
char ByteToChar(string str);
void convertBinToString(vector<int> &quadro, string &mensagem);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(vector<int> &quadro, int paridade);
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro, int polinomio[]);
vector<int> calculo_crc(vector<int> quadro, int polinomio[]);
void CamadaEnlaceDadosReceptoraControleDeErro(vector<int> &quadro);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> &quadro);
void CamadaEnlaceDadosReceptoraControledeErroBitParidadeImpar(vector<int> &quadro);
void CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro);


/*
    Realiza a chamada da primeira camada, onde começa todo o processo
*/
int main() {
    cout << "-------------------Camada de enlace simulacao---------------------\n";
    AplicacaoTransmissora();
}

/*
    Simula o comportamento do computador A, que envia a mensagem
*/
void AplicacaoTransmissora() {
    
    string mensagem;
    cout << "-------------------Computador A ----------------------------------\n";
    cout << "Digite uma mensagem: " << endl;
    getline( cin, mensagem );//realiza leitura de mensagem com espaco
    //chama proxima camada
    CamadaDeAplicacaoTransmissora(mensagem); // em um exemplo mais realistico, aqui sera dado um SEND do Socket
} 

/*
    Realiza a transformação da mensagem recebida em String para bits
    mensagem: recebe a mensagem em string 
*/
void CamadaDeAplicacaoTransmissora(string mensagem) {
    vector<int> quadro; //vetor responsável por guardar a mensagem em bits
    
    string_to_bin(mensagem, quadro);//Transforma a mensagem em bits
    
    CamadaEnlaceDadosTransmissora(quadro);//chama a próxima camada
    
} 

/*
    Recebe a mensagem em bits chama a CamadaEnlaceDadosTransmissoraControleDeErro, que roda um algoritmo
    de controle de erro e chama a próxima camada (Meio de Comunicacao)
    quadro: mensagem em bits
*/
void CamadaEnlaceDadosTransmissora(vector<int> &quadro) {
    
    CamadaEnlaceDadosTransmissoraControleDeErro(quadro);//roda um algoritmo de controle de erro

    MeioDeComunicacao(quadro);//chama a proxima camada
   
} 

/*
    Recebe a mensagem em bits, e faz o faz o tratamento da mensagem com um algoritmo de controle de erro
    seleciona pela variavel tipoDeControle
    quadro: mensagem em bits
*/
void CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> &quadro){
    int tipoDeControle = 2; //define qual algoritmo será usado
    if(tipoDeControle == 0)
        cout << "\n-------------------METODO PARIDADE PAR--------------------------------\n";
    if(tipoDeControle == 1)
        cout << "\n-------------------METODO PARIDADE IMPAR--------------------------------\n";
    if(tipoDeControle == 2)
        cout << "\n-------------------METODO CRC--------------------------------------------\n";
    //polinomio CRC-POLYNOMESIZE (IEEE 802)
    int polinomio_crc[POLYNOMESIZE] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1}; 
    vector<int> resto;//polinomio CRC
    switch (tipoDeControle) {
    case 0: //bit de paridade par
        CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(quadro, 0);
        break;
    case 1: //bit de paridade impar
        CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(quadro, 1);
        break;
    case 2: //CRC
        resto = CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro, polinomio_crc);
        for (int i = 0; i < POLYNOMESIZE - 1; i++)
            quadro.push_back(resto[i]); // Adiciona CRC
        break;
    }
}

/*
    Simula o Meio de comunicacao fisico, em que erros podem acontecer
    durante a transmissao de uma mensagem, a porcetagem de erros que se deseja que
    aconteca e selecionada na variavel porcentagem
    fluxoBrutoDeBits: mensagem a ser transmitida 
*/
void MeioDeComunicacao(vector<int> &fluxoBrutoDeBits) {
    float porcentagemDeErros;
    int BitsSize = fluxoBrutoDeBits.size();
    int fluxoBrutoDeBitsPontoA[BitsSize];
    vector<int> fluxoBrutoDeBitsPontoB;
    int progresso = 0;

    porcentagemDeErros = 0; // 10% 20%, 30%, 40%, ... 100%
    
    for (int i = 0; i < fluxoBrutoDeBits.size(); i++) {
        fluxoBrutoDeBitsPontoA[i] = fluxoBrutoDeBits[i];
    }

    while (progresso < BitsSize) {
        int bit = fluxoBrutoDeBitsPontoA[progresso++];
        //se o valor aleatorio gerado for menor que a porcetagem de erros, um erro eh gerado
        if (rand() % 100 < porcentagemDeErros) {
            bit = (bit + 1) % 2;//geracao de erro
        }
        fluxoBrutoDeBitsPontoB.push_back(bit);
    }

    CamadaEnlaceDadosReceptora(fluxoBrutoDeBitsPontoB);
}

/*
    Recebe a mensagem em bits, realiza a chamada 
    CamadaEnlaceDadosReceptoraControleDeErro que realizara a verificacao se
    existe um erro na transmissao da mensagem ou nao e chama a proxima camada
    quadro: mesangem em bits
*/
void CamadaEnlaceDadosReceptora(vector<int> &quadro) {
    
    CamadaEnlaceDadosReceptoraControleDeErro(quadro);//verificacao de erro
    
    CamadaAplicacaoReceptora(quadro);//proxima camada
}

/*
    Recebe a mensagem em Bits, traduz eles para String e chama a proxima camada
    quadro: mensagem em bits
*/
void CamadaAplicacaoReceptora(vector<int> &quadro){

    string mensagem = "";
    convertBinToString(quadro, mensagem);//conversao de quadro (bits) para mensagem(string)
    
    AplicacaoReceptora(mensagem);//proxima camada
}

/*
    Simula o comportamento do computador B, que recebe a mensagem
    mensagem: recebe a mensagem
*/
void AplicacaoReceptora(string mensagem) {
    cout << "\n-------------------Computador B ----------------------------------\n";
    cout << "A mensagem recebida foi:" << mensagem << endl;
} 

/*
    Função que converte string 'mensagem' para binario e armazena em 'quadro'
*/
void string_to_bin(string mensagem, vector<int> &quadro) {
    string binaryString = "";

    for (int i = 0; i < mensagem.size(); i++)
        binaryString += (bitset<8>(mensagem[i]).to_string());
    
    //cada posicao do vetor armazena um bit
    for (int i = 0; i < binaryString.size(); i++)
        quadro.push_back(binaryString[i] - '0');
}

/*
    Converte um byte em char
    string str = um caracter da mensagem em bytes
*/
char ByteToChar(string str) {
    char parsed = 0;
    for (int i = 0; i < 8; i++) {
        if (str[i] == '1')
            parsed |= 1 << (7 - i);
    }
    return parsed;
}

/*
    Converte uma mensagem em binario para string
    quadro: mensagem em binario
    mensagem: variavel que recebera o valor da mensagem em string
*/
void convertBinToString(vector<int> &quadro, string &mensagem) {
    string binaryString = "";

    for (int i = 0; i < quadro.size(); i++)
        binaryString += quadro[i] + '0';

    for (int i = 0; i < binaryString.size(); i += 8)
        mensagem += ByteToChar(binaryString.substr(i, i + 8));

}


/*
    Roda o algoritmo de Paridade para controle de Erro
    quadro: vetor com os bits
    paridade: se deseja que o algoritmo seja de paridade par(0) ou paridade impar(1)
*/
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(vector<int> &quadro, int paridade) {
    int qtdd1 = 0;

    for (int i = 0; i < quadro.size(); i++) {
        if (quadro[i])
            qtdd1++;
    }

    if (qtdd1 % 2 == paridade)
        quadro.push_back(0);
    else
        quadro.push_back(1);

}

/*
    Roda o algoritmo de CRC para controle de Erro
    quadro: vetor com os bits
    polinomio: polinomio CRC
*/
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro, int polinomio[]) {
    vector<int> resto;
    for (int i = 0; i < POLYNOMESIZE - 1; i++)
        quadro.push_back(0);

    resto = calculo_crc(quadro, polinomio); //funcao que calcula a funcao CRC

    return resto;
}

/* 
    Realiza o calculo da funcao CRC
    quadro: vetor com os bits
    polinomio: polinomio CRC
*/
vector<int> calculo_crc(vector<int> quadro, int polinomio[]) {
    vector<int> resto;
    for (int i = 0; i < quadro.size() - (POLYNOMESIZE - 1); i++) {
        if (polinomio[0] == quadro[i]) {
            for (int j = 0, k = i; j < POLYNOMESIZE; j++, k++) {
                if (!(quadro[k] ^ polinomio[j]))
                    quadro[k] = 0;
                else
                    quadro[k] = 1;
            }
        }
    }
    for (int i = quadro.size() - (POLYNOMESIZE - 1); i < quadro.size(); i++)
        resto.push_back(quadro[i]);

    return resto;
}


/*
    Recebe a mensagem em bits e de acordo com o algoritmo de verificao de erro usado 
    no envio da mensagem, realiza se houve ou nao erro na transmissao
    quadro: mensagem em bits
*/
void CamadaEnlaceDadosReceptoraControleDeErro(vector<int> &quadro) {
    int tipoDeControle = 2;//escolhe algoritmo de verificao de erro  
    switch (tipoDeControle){                                                                              
    case 0: //bit de paridade par
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
        quadro.pop_back();
        break;
    case 1: //bit de paridade impar
        CamadaEnlaceDadosReceptoraControledeErroBitParidadeImpar(quadro);
        quadro.pop_back();
        break;
    case 2: //CRC
        CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
        for(int i = 0; i < POLYNOMESIZE - 1; i++)
            quadro.pop_back();
        break;
    }
}

/*
    Recebe a mensagem em bits e realiza a verificao de acordo com o algoritmo de controle
    de erro de bit de paridade par
    quadro: mensagem em bits
*/
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> &quadro) {
    int qtdd1 = 0;

    for (int i = 0; i < quadro.size() - 1; i++)
        if (quadro[i])
            qtdd1++;

    if (qtdd1 % 2 == 0 && quadro[quadro.size() - 1] == 1)
        cout << "\n!!!!!!!!!!!!!!!!!!ERRO DE PARIDADE PAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    else if (qtdd1 % 2 == 1 && quadro[quadro.size() - 1] == 0)
        cout << "\n!!!!!!!!!!!!!!!!!!ERRO DE PARIDADE PAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
}

/*
    Recebe a mensagem em bits e realiza a verificao de acordo com o algoritmo de controle
    de erro de bit de paridade impar
    quadro: mensagem em bits
*/

void CamadaEnlaceDadosReceptoraControledeErroBitParidadeImpar(vector<int> &quadro) {
    int qtdd1 = 0;
   
    for (int i = 0; i < quadro.size() - 1; i++)
        if (quadro[i])
            qtdd1++;

    if (qtdd1 % 2 == 0 && quadro[quadro.size() - 1] == 0)
        cout << "\n!!!!!!!!!!!!!!!!!!ERRO DE PARIDADE IMPAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    else if (qtdd1 % 2 == 1 && quadro[quadro.size() - 1] == 1)
        cout << "\n!!!!!!!!!!!!!!!!!!ERRO DE PARIDADE IMPAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
}

/*
    Recebe a mensagem em bits e realiza a verificao de acordo com o algoritmo de controle
    de erro de bit de CRC
    quadro: mensagem em bits
*/
void CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro) {
    vector<int> resto;
    int polinomio_crc[POLYNOMESIZE] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1}; //polinomio CRC-POLYNOMESIZE (IEEE 802)
    resto = calculo_crc(quadro, polinomio_crc);
    //Se a funcao do polinomio nao for 0 apos o calculo CRC e porque possui erro
    for (int i = 0; i < POLYNOMESIZE; i++) 
        if(resto[i]) {
            cout << "\n!!!!!!!!!!!!!!!!!!ERRO DE CRC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            break;
        }
}