package HotelAPI.model;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDate;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "postagem")
public class Postagem {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id_postagem;

    @Column(name = "coteudo")
    private String conteudo;
    @Column(name = "tipo")
    private String tipo;
    @Column(name = "data")
    private LocalDate data;

    @ManyToOne
    @JoinColumn
    private Usuario usuario;



}
