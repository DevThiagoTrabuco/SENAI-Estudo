package com.example.jpa_relacionamentos.entidades;

import jakarta.persistence.*;
import lombok.*;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Entity
public class CursoEstudante {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id_curso_estudante")
    private int id;

    @ManyToOne
    @JoinColumn(name = "id_curso", referencedColumnName = "id_curso")
    private Curso curso;

    @Column(name = "id_curso")
    private int cursoId;
    
    @ManyToOne
    @JoinColumn(name = "id_estudante", referencedColumnName = "id_estudante")
    private Estudante estudante;

    @Column(name ="id_estudante")
    private int estudanteId;
}
