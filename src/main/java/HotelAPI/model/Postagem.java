package HotelAPI.model;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.time.LocalDateTime;  // Mude de LocalDate para LocalDateTime
import java.util.List;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "postagem")
public class Postagem {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id_postagem")
    private Integer idPostagem;  // Mude para Integer e camelCase

    @Column(name = "conteudo", columnDefinition = "TEXT")
    private String conteudo;

    @Column(name = "tipo", length = 50)
    private String tipo;

    @Column(name = "data_publicacao")  // Nome correto da coluna
    private LocalDateTime dataPublicacao;  // Mude para LocalDateTime

    @ManyToOne
    @JoinColumn(name = "id_usuario", nullable = false)  // Adicione esta anotação
    private Usuario usuario;

    @OneToMany(mappedBy = "postagem", cascade = CascadeType.ALL)
    private List<Comentario> comentarios;

    // Adicione este método para setar a data automaticamente antes de salvar
    @PrePersist
    public void prePersist() {
        if (dataPublicacao == null) {
            dataPublicacao = LocalDateTime.now();
        }
    }
}