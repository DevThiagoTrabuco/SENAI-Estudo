package com.example;

public class Circulo implements FormaGeometrica{
    private double raio;
    

    public Circulo(double raio) {
        this.raio = raio;;
    }

    public double getRaio(){
        return raio;
    }

    @Override
    public double calcularArea(){
        return Math.PI * Math.pow(raio, 2);
    }
}
