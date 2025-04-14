package com.example;

public class Quadrado implements FormaGeometrica{
    private double lado;

    public Quadrado(double lado) {
        this.lado = lado;
    }

    public double getLado(){
        return lado;
    }

    @Override
    public double calcularArea() {
        return Math.pow(lado ,2);
    }
}
