#include <iostream>
#include <myo/myo.hpp>
#include "DataCollector.cpp"

int main(int argc, const char * argv[]) {
    
    myo::Hub hub("com.pucminas.computacaoubiqua.MyoMacControl");
    
    std::cout << "Iniciando conexao com o Myo..." << std::endl;
    
    myo::Myo* myo = hub.waitForMyo(10000);
    
    if (!myo) {
        throw std::runtime_error("Erro ao conectar com o Myo!");
    }
    
    std::cout << "Conexão com o Myo com sucesso!" << std::endl << std::endl;
    
    DataCollector collector;
    hub.addListener(&collector);
    
    while (1) {
        hub.run(1000/20);
        collector.process();
    }
    
    return 0;
}
