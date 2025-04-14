package com.example.jpa_relacionamentos.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.jpa_relacionamentos.entidades.CursoEstudante;
import com.example.jpa_relacionamentos.repositories.CursoEstudanteRepository;

@RequestMapping("/api/cursoestudante/")
@RestController
public class CursoEstudanteController {
    private final CursoEstudanteRepository cursoestudanteRepository;

    @Autowired
    public CursoEstudanteController(CursoEstudanteRepository cursoestudanteRepository) {
        this.cursoestudanteRepository = cursoestudanteRepository;
    }

    @GetMapping("")
    public List<CursoEstudante> getAll(){
        return cursoestudanteRepository.findAll();
    }

    @GetMapping("/{codigo}")
    public CursoEstudante getById(@PathVariable int codigo){
        return cursoestudanteRepository.findById(codigo).get();
    }

    @PostMapping("")
    public void save(@RequestBody CursoEstudante cursoestudante){
        cursoestudanteRepository.save(cursoestudante);
    }

    @PutMapping("/{codigo}")
    public void update(@PathVariable int codigo, @RequestBody CursoEstudante cursoestudante){
        cursoestudanteRepository.save(cursoestudante);
    }

    @DeleteMapping("/{codigo}")
    public void delete(@PathVariable int codigo){
        cursoestudanteRepository.deleteById(codigo);
    }
}
