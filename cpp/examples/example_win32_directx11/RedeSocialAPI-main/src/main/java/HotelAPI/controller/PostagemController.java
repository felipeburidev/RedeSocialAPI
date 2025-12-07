package HotelAPI.controller;

import HotelAPI.model.Postagem;
import HotelAPI.model.Usuario;
import HotelAPI.repository.PostagemRepository;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/posts")
@CrossOrigin("*")
public class PostagemController {

    private final PostagemRepository postagemRepository;

    public PostagemController(PostagemRepository postagemRepository) {
        this.postagemRepository = postagemRepository;
    }

    //Get em todos
    @GetMapping
    public List<Postagem> findAll() {
        return postagemRepository.findAll();
    }

    //Get por id
    @GetMapping("{id}")
    public Postagem findById(@PathVariable int id) {
        return postagemRepository.findById(id).orElse(null);
    }

    //Post
    @PostMapping("Post")
    public Postagem save(@RequestBody Postagem postagem) {
        return postagemRepository.save(postagem);
    }

    //Deletar por id
    @DeleteMapping("delete{id}")
    public void deleteById(@PathVariable int id) {
        postagemRepository.deleteById(id);
    }

    //Put por id
    @PutMapping("/alter{id}")
    public Postagem update(@RequestBody Postagem postagem, @PathVariable int id) {
        return postagemRepository.save(postagem);
    }


}
