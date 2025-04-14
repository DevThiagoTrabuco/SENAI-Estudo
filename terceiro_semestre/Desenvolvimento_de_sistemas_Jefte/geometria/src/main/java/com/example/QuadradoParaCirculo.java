package com.example;

public class QuadradoParaCirculo implements FormaGeometrica{
    private Quadrado quadrado;
    private Circulo circulo;

    public QuadradoParaCirculo(Quadrado quadrado) {
        this.quadrado = quadrado;
        this.circulo = new Circulo(this.quadrado.getLado());
    }

    public double getLado(){
        return quadrado.getLado();
    }

    @Override
    public double calcularArea(){
        return circulo.calcularArea();
    }
}
