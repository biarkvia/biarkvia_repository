<?php

namespace App\Form;

use App\Entity\Account;
use App\Entity\Employee;
use Symfony\Bridge\Doctrine\Form\Type\EntityType;
use Symfony\Component\Form\AbstractType;
use Symfony\Component\Form\Extension\Core\Type\ChoiceType;
use Symfony\Component\Form\Extension\Core\Type\DateType;
use Symfony\Component\Form\Extension\Core\Type\SubmitType;
use Symfony\Component\Form\Extension\Core\Type\TextType;
use Symfony\Component\Form\FormBuilderInterface;
use Symfony\Component\OptionsResolver\OptionsResolver;
use Symfony\Component\Validator\Constraints\NotBlank;

class CreateAccountType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $isEdit = $options['is_edit'] ?? false;

        $builder
            ->add('name', TextType::class, [
                'label' => 'Název účtu',
                'constraints' => [new NotBlank()],
            ])
            ->add('type', ChoiceType::class, [
                'label' => 'Typ účtu',
                'choices' => [
                    'Username' => 'Username',
                    'Password' => 'Password',
                    'Card' => 'Card',
                ],
                'expanded' => true,
                'constraints' => [new NotBlank()],
                'attr' => ['class' => 'custom-block'],
            ])
            ->add('expiration', DateType::class, [
                'label' => 'Expirace',
                'widget' => 'single_text',
                'required' => false,
                'input' => 'datetime',
                'format' => 'yyyy-MM-dd',
                'html5' => true,
            ])
            ->add('employee', EntityType::class, [
                'class' => Employee::class,
                'choice_label' => 'id',
                'label' => 'ID zaměstnance',
                'placeholder' => 'Vyberte zaměstnance',
            ])
            ->add('submit', SubmitType::class, [
                'label' => $isEdit ? 'Úpravit účet' : 'Vytvořit účet',
                'attr' => ['class' => 'btn-primary'],
            ]);
    }

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults([
            'data_class' => Account::class,
            'is_edit' => false,
        ]);
    }
}