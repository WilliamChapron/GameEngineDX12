

//https://www.braynzarsoft.net/viewtutorial/q16390-04-directx-12-braynzar-soft-tutorials

 /*
    ***********************
    CODE STYLE DESCRIPTION
    ***********************
*/

// - Pascal Case For Class 
// - Pascal Case For Functions
// - Camel Case For Var 
// - Add _type after normal var (facultatif for not important type but don't need to put "_" if no type write) 
// - Add type_ before class member var (facultatif for not important type but need always put the "_") 
// - Don't need specifiy _ or type with args var
// - Multiple Line commentary for Functions or Methods decription, like : ( /* - Commentary - */ )
// - One line commentary for one line Operation, next to the line, like : ( // $Commentary )
// - To Think commentary : #TOTHINK - $
// - To Do commentary : #TODO - $

/*
    CODE STYLE DESCRIPTION
*/



#include <iostream>  
#include <Windows.h>  
#include "stdafx.h"   
#include "WindowInitializer.h"
#include "DirectInitializer.h"
#include "EngineManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    EngineManager::Initialize(&hInstance, nShowCmd);

    // Ferme la console avant la fin du programme
    /*engineInstance->Cleanup();
    
    delete engineInstance;*/
    return 0;
}










// Créer une classe a vocation d'être instancié une seul fois et qui donne accès a des classes et variables statiques


//class Manager 
//{
//private:
//    static Manager* pInstance;
//    Manager() {}
//
//public:
//    static void Init() 
//    {
//        if (pInstance != nullptr)
//            return;
//
//        pInstance = new Manager();
//    }
//
//    static Manager* Get() 
//    {
//        return pInstance;
//    }
//
//private:
//    int i;
//    int j;
//
//public:
//    int function();
//
//};
//
//
//Manager::Init();
//Manager::Get()->function();



// Créer un classe normale mais ajouter a souhait des attributs et methode accessible par tous en statique 


//class MaClasse {
//public:
//    // Méthode non statique
//    void methodeNonStatique() {
//        std::cout << "Méthode non statique" << std::endl;
//    }
//
//    // Méthode statique
//    static void methodeStatique() {
//        std::cout << "Méthode statique" << std::endl;
//    }
//
//    // Accesseur pour une variable non statique
//    int getVariableNonStatique() const {
//        return variableNonStatique;
//    }
//
//    // Accesseur pour une variable statique
//    static int getVariableStatique() {
//        return variableStatique;
//    }
//
//private:
//    // Variable non statique
//    int variableNonStatique;
//
//    // Variable statique
//    static int variableStatique;
//};
//
//// Initialisation de la variable statique
//int MaClasse::variableStatique = 0;
//
//int main() {
//    // Utilisation avec une instance
//    MaClasse obj;
//    obj.methodeNonStatique();          // Appel de la méthode non statique
//    std::cout << obj.getVariableNonStatique() << std::endl;  // Accès à la variable non statique
//
//    // Utilisation sans instance
//    MaClasse::methodeStatique();       // Appel de la méthode statique
//    std::cout << MaClasse::getVariableStatique() << std::endl;  // Accès à la variable statique
//
//    return 0;
//}