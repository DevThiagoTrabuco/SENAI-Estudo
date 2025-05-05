package com.example.security.entities;

import jakarta.persistence.Entity;
import jakarta.persistence.Table;
import lombok.*;

@Setter
@Getter
@AllArgsConstructor
@NoArgsConstructor
@Entity
@Table(name = "papeis")
public class Papel {
    private int id;
    private String nome;
}
