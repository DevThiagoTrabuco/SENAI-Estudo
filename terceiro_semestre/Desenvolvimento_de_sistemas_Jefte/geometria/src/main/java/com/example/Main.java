package com.example;

public class Main {
    public static void main(String[] args) {
        Quadrado quadrado = new Quadrado(2);
        Circulo circulo = new Circulo(2);
        QuadradoParaCirculo quadradoParaCirculo = new QuadradoParaCirculo(quadrado);
        System.out.println("Área do quadrado: " + quadrado.calcularArea());
        System.out.println("Área do círculo: " + circulo.calcularArea());
        System.out.println("Área do cículo: " + quadradoParaCirculo.calcularArea());
    }
}